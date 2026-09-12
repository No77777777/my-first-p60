#=========================================================================
#
# UCLIB Config Generator
#
# Copyright (c) 2017 - 2021 Qualcomm Technologies, Inc. and/or its subsidiaries.
# All Rights Reserved.
# Confidential and Proprietary - Qualcomm Technologies, Inc.
#
#=========================================================================
'''
    Created on Oct 4th, 2016
    @authors: bsharma

    Python Script for auto generating dependencies and config files needed to build
    uclib. The files are generated during build time.

    Files generated:
    /uclib/uclib/build/<tgt_img>/sources_aarch64.cfg
    /uclib/uclib/build/<tgt_img>/sources_aarch32.cfg
    /uclib/uclib/cfg/cfg.h
    /uclib/uclib/cfg/<tgt_img>/cfg32.c
    /uclib/uclib/cfg/<tgt_img>/cfg64.c

'''

import os
import json
from collections import OrderedDict


COMMENT_START_LINE = "/*" + "=" * 80
COMMENT_END_LINE = "=" * 80 + "*/"
SUCCESS = 0
ERROR = 1


class g(object):
    generated_files = []
    heap_needed = False
    arch = None
    offtarget = None
    debug_build = False
    ctx_type = {
        'HASH': 'hash_ctx_t',
        'CIPHER': 'cipher_ctx_t',
        'KDF': 'kdf_ctx_t',
        'MAC': 'mac_ctx_t',
        'RAND': '',
        'PKEY': 'pkey_ctx_t',
    }
    all_categories = ["HASH", "CIPHER", "KDF", "MAC", "PKEY", "RAND"]
    supported_images = ['arm_tz', 'arm_usr', 'x86_tz', 'x86_usr', 'x86_uctest',
        'dsp_mpss', 'dsp_mba', 'arm_pbl', 'arm_xblcore', 'arm_xblloader', 'arm_xbldevprog', 'arm_xblramdump', 'riscv_tmepbl']

#------------------------------------------------------------------------------
# Hooks for Scons
#------------------------------------------------------------------------------


def exists(env):
    return env.Detect('config_generator')


def generate(env):
    env.AddMethod(generate_all_files, 'generate_uclib_configs')
    return

#=========================================================================
# generate all config files
#=========================================================================

def generate_all_files(env, arch, buildpath):
    """ auto-generates the following files for uclib compilation
            /uclib/uclib/build/<tgt_img>/sources_aarch<64/32>.cfg
            /uclib/uclib/cfg/cfg.h
            /uclib/uclib/cfg/<tgt_img>/cfg<64/32>.c
        :param env:         Scons environment
        :param arch:        32 or 64 bit architecture
        :param buildpath:   Buildpath of the image variant where we want to
                            auto-generate required files. For non-scons build
                            environments or standalone usecase, user should provide
                            some directory that is included during compilation.
    """

    tgt_image = env['TARGET_DESTINATION']
    uclib_path = env['UCLIB_PATH']
    g.arch = arch

    if tgt_image not in g.supported_images:
        raise Exception("Error: Unsupported tgt_image - {}\nSupported values: {}"
            .format(tgt_image, g.supported_images))

    g.offtarget = 'x86_' in tgt_image
    g.proc = tgt_image.split('_')[0]

    ## Output files
    g.generated_files = []
    sources_f = os.path.join(buildpath, 'sources_aarch' + g.arch + '.cfg')
    cfg_h_f = os.path.join(buildpath, 'cfg.h')
    cfg_c_f = os.path.join(buildpath, 'cfg' + g.arch + '.c')

    ## Input files
    config, deps, eng_entries, tgt_info = load_all_json_files(tgt_image, uclib_path)

    if tgt_info is None:
        g.engines_supported = ['SW', 'ARMV8', 'BAM', 'HWIO', 'PKA', 'HRNG', 'TME_SEQ', 'ARMV8_2']
    elif g.offtarget:
        g.engines_supported = tgt_info["engines_supported"]['offtarget']['engines']
    else:
        try:
            g.engines_supported = tgt_info["engines_supported"][env['CHIPSET']]['engines']
        except KeyError:
            print("Warning: Chipset {} not found in target_info.json, falling back to stub".format(env['CHIPSET']))
            g.engines_supported = tgt_info["engines_supported"]['stub']['engines']

    ret = generate_sources_files(env, config, deps, sources_f, buildpath)
    ret = ret or generate_cfg_files(config, deps, eng_entries, cfg_c_f, cfg_h_f)

    if ret and g.generated_files:
        print("Error generating required files, deleting all generated files")
        delete_files(g.generated_files)

    return g.generated_files


def load_all_json_files(tgt_image, uclib_path):
    uc_par_dir = os.path.dirname(uclib_path)
    img_config_paths = {
        'arm_tz':       os.path.join(uc_par_dir, 'ssg', 'securemsm', 'uclib', 'config'),
        'arm_usr':      os.path.join(uc_par_dir, 'ssg', 'securemsm', 'uclib', 'config'),
        'x86_tz':       os.path.join(uc_par_dir, 'ssg', 'securemsm', 'uclib', 'config'),
        'x86_usr':      os.path.join(uc_par_dir, 'ssg', 'securemsm', 'uclib', 'config'),
        'dsp_mpss':     os.path.join(uc_par_dir, 'uclib_mpss', 'config'),
        'dsp_mba':      os.path.join(uc_par_dir, 'uclib_mpss', 'config'),
        'riscv_tmepbl': os.path.join(uc_par_dir, 'pbl'),
        'x86_uctest':   os.path.join(uc_par_dir, 'uclib', 'uctest', 'config'),
        'default':      os.path.join(uclib_path, 'bsp', 'uclib', 'cfg'),
    }

    config_filename = tgt_image.split('_')[1] + '_config.json'

    if tgt_image not in list(img_config_paths.keys()):
        tgt_image = 'default'
    config_file = os.path.join(img_config_paths[tgt_image], config_filename)
    if not os.path.exists(config_file):
        raise IOError("File doesn't exist - {}".format(config_file))

    if tgt_image in ['default']:
        target_info_file = None
    else:
        target_info_file = os.path.join(img_config_paths[tgt_image], 'target_info.json')
        if not os.path.exists(target_info_file):
            raise IOError("File doesn't exist - {}".format(target_info_file))

    all_dependencies_file = os.path.join(img_config_paths['default'], 'all_dependencies.json')
    engine_entries_file = os.path.join(img_config_paths['default'], 'engine_entries.json')

    try:
        # load image specific json config file
        json_file = config_file
        with open(json_file) as cfg_file:
            config = json.load(cfg_file, object_pairs_hook=OrderedDict)

        # load json file with all uclib sources/dependencies
        json_file = all_dependencies_file
        with open(json_file) as deps_file:
            deps = json.load(deps_file)

        # load json file with engine entries
        json_file = engine_entries_file
        with open(json_file) as eng_file:
            eng_entries = json.load(eng_file)

        # load json file with target specific info
        if target_info_file is None:
            tgt_info = None
        else:
            json_file = target_info_file
            with open(json_file) as tgt_file:
                tgt_info = json.load(tgt_file)

    except (ValueError, Exception) as e:
        raise ValueError('\nError loading JSON file {}\nError Details: {}\n'.format(json_file, str(e)))

    return config, deps, eng_entries, tgt_info


def generate_sources_files(env, config, deps, sources_f, buildpath):
    """ Generates a file with list of dependencies or sources needed to build an image
        file - sources_aarch32.cfg or sources_aarch64.cfg depending on arch
    """

    # check if image name is provided
    try:
        key = "heap-lite-required"
        g.heap_needed = config.pop(key)
    except KeyError:
        print("Key {} not provided in config file".format(key))
        return ERROR

    e_arch = get_arch()
    eng_map = config['offtarget-engine-interface-vs-implementation-mapping']

    # Workaround for TME-PBL pre-sil to unblock secboot validation
    riscv_wa = e_arch.lower().startswith("riscv")

    # Main Loop for generating dependencies file
    with open(sources_f, 'w') as depfile:
        g.generated_files.append(sources_f)
        # add common dependencies
        depfile.write("#****** THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY ******#\n")
        depfile.write("#common_dependencies")
        cmn_d = get_dependencies(deps, e_arch)

        # correct paths for cfg64.c or cfg32.c
        for line in cmn_d:
            if "<build_path>" in line:
                cmn_d.remove(line)
                line = line.replace("<build_path>", buildpath)
                cmn_d.append(line)

        tests = []
        # add heap lite if needed
        if g.heap_needed:
            cmn_d.extend(deps["heap-lite"])
            if "heap" in list(deps["TESTS"].keys()):
                tests.extend(deps["TESTS"]["heap"])
        # add logging if needed
        if config["uclib-debug-build"]:
            env.Append(CPPDEFINES=["UCLIB_DBG_BUILD"])
            cmn_d.extend(deps["logging"])
        depfile.write("\n" + "\n".join(cmn_d))
        hwio, bam = False, False
        all_src_files = []

        # for each category, find the enabled configuration and get
        # dependencies in an array
        for cat in g.all_categories:
            all_src_files.append("\n\n#" + cat.lower() + "_dependencies")
            cat_d = []
            if cat not in list(config.keys()):
                continue
            cat_d = get_dependencies(deps[cat], e_arch)
            sha2testupdated = 0
            for algo in list(config[cat].keys()):
                if len(list(config[cat][algo].keys())) == 0:
                    continue
                cat_d.extend(get_dependencies(deps[cat][algo], e_arch))
                if algo in ["AES", "HC", "DES", "2DES", "TDES", "SM4", "STREAM", "CHACHA20"]:  # or algo == "KBKDF":
                    if algo == "AES" and "common" in list(deps["TESTS"][cat][algo].keys()):
                        tests.extend(deps["TESTS"][cat][algo]["common"])
                    # mode = AES-xxx, DES-xxx, 2DES-xxx, TDES-xxx, HC-xxx
                    for mode in list(config[cat][algo].keys()):
                        if len(list(config[cat][algo][mode].keys())) == 0:
                            continue
                        cat_d.extend(get_dependencies(
                            deps[cat][algo][mode], e_arch))
                        for engine in config[cat][algo][mode][e_arch]:
                            if g.offtarget or (riscv_wa and "HASH" == cat):
                                engine = eng_map[engine]
                            if engine == 'HWIO':
                                hwio = True
                            elif engine == 'BAM':
                                bam = True
                            if engine in g.engines_supported:
                                cat_d.extend(get_dependencies(
                                    deps[cat][algo][mode][engine], e_arch))
                        if algo == "AES" and mode in list(deps["TESTS"][cat][algo].keys()):
                            tests.extend(deps["TESTS"][cat][algo][mode])
                else:
                    for engine in config[cat][algo][e_arch]:
                        if g.offtarget or (riscv_wa and "HASH" == cat):
                            engine = eng_map[engine]
                        if engine == 'HWIO':
                            hwio = True
                        elif engine == 'BAM':
                            bam = True
                        if engine in g.engines_supported:
                            cat_d.extend(get_dependencies(
                                deps[cat][algo][engine], e_arch))
                    if algo in list(deps["TESTS"][cat].keys()):
                        tests.extend(deps["TESTS"][cat][algo])
                    elif "SHA2" in algo and sha2testupdated == 0:
                        tests.extend(deps["TESTS"][cat]["SHA2"])
                        sha2testupdated = 1
            # cleanup duplicates from the array and write to output file
            if cat_d is not None:
                all_src_files.extend(cat_d)
        hw_d = []
        if hwio or bam:
            hw_d.append("\n#HW engine dependencies")
            if hwio:
                hw_d.extend(deps["engine"]["HWIO"])
            if bam:
                hw_d.extend(deps["engine"]["BAM"])
        all_src_files.extend(hw_d)

        # also write the test sources to the output file
        all_src_files.append("\n\n#tests")
        if tests is not None:
            tests.extend(deps["TESTS"]["selftest"])
            all_src_files.extend(tests)
        all_src_files = remove_duplicates(all_src_files)
        depfile.write("\n".join(all_src_files))
    return SUCCESS


def get_dependencies(deps, e_arch):
    """ Get common dependencies and 64-bit or 32-bit dependencies for a node """
    dependencies = []
    try:
        if "common" in list(deps.keys()):
            dependencies.extend(deps["common"])
        if e_arch.endswith("_64") and "64-bit" in list(deps.keys()):
            dependencies.extend(deps["64-bit"])
        elif e_arch.endswith("_32")  and "32-bit" in list(deps.keys()):
            dependencies.extend(deps["32-bit"])
    except KeyError:
        None
    return dependencies


def generate_cfg_files(config, deps, eng_entries, cfg_c_f, cfg_h_f):
    """ Generates config files - cfg.h and cfg.c """

    # extract list of categories to loop over
    try:
        key = "uclib-debug-build"
        g.debug_build = config.pop(key)
    except KeyError:
        print("Key {} not provided in config file".format(key))
        return ERROR

    try:
        with open(cfg_h_f, 'w') as hfile, open(cfg_c_f, 'w') as cfile:
            g.generated_files.append(cfg_h_f)
            g.generated_files.append(cfg_c_f)
            if gen_cfg_heading(hfile, cfile):
                return ERROR
            if gen_cfg_includes(hfile, cfile, deps):
                return ERROR
            if gen_cfg_h_macros(hfile, deps):
                return ERROR
            if gen_cfg_c_macros(cfile, config, deps):
                return ERROR
            if gen_cfg_c_engine_entries(cfile, config, eng_entries):
                return ERROR
            if gen_cfg_functions(hfile, cfile, config):
                return ERROR
            hfile.write("\n\n#endif /* __UCLIB_PRV_CFG */")

            # end files with newline to satisfy non-c11 compliant compilers!
            hfile.write("\n")
            cfile.write("\n")
            return SUCCESS
    except IOError as e:
        print("Opening file failed: {}".format(e))
        return ERROR


def gen_cfg_functions(hfile, cfile, config):
    """ generate function declarations and definitions in cfg.h and cfg.c resp """

    definitions = []
    fn_decl = ""
    declarations = []
    for c in config['supported-categories']:
        fn_decl = "UCLIB_ENGINE_TYPE CFG_get_default_" + c.lower() + "_engine(void)"
        declarations.append(fn_decl + ";")
        definitions.append(
            "\n" + fn_decl + "\n{\n\treturn DEFAULT_" + c + "_ENGINE;\n}")

    fn_decl = "void *CFG_get_prng_data_cb(void)"
    declarations.append(fn_decl + ";")
    definitions.append(
        "\n" + fn_decl + "\n{\n\treturn DEFAULT_get_prng_data_cb;\n}")

    ret = "uc_mem_init_heap" if g.heap_needed else "NULL"
    fn_decl = "void *CFG_get_uc_heap_init_cb(void)"
    declarations.append(fn_decl + ";")
    definitions.append("\n" + fn_decl + "\n{\n\treturn " + ret + ";\n}")

    ret = "uc_mem_malloc" if g.heap_needed else "NULL"
    fn_decl = "void *CFG_get_mem_uc_malloc_cb(void)"
    declarations.append(fn_decl + ";")
    definitions.append("\n" + fn_decl + "\n{\n\treturn " + ret + ";\n}")

    ret = "uc_mem_free" if g.heap_needed else "NULL"
    fn_decl = "void *CFG_get_mem_uc_free_cb(void)"
    declarations.append(fn_decl + ";")
    definitions.append("\n" + fn_decl + "\n{\n\treturn " + ret + ";\n}")

    fn_decl = "bool CFG_is_prng_init_required(void)"
    declarations.append(fn_decl + ";")
    definitions.append("\n" + fn_decl + "\n{\n\treturn true;\n}")

    for c in config['supported-categories']:
        fn_decl = c.lower() + "_init_fp CFG_get_" + c.lower() + "_init_entry(UCLIB_" + c + "_ALG alg, " \
            + ("UCLIB_CIPHER_MODE mode, " if c == "CIPHER" else "") + "UCLIB_ENGINE_TYPE engine)"
        declarations.append(fn_decl + ";")
        definitions.append("\n" + fn_decl + "\n{")
        if c != "KDF":
            definitions.append("\tUC_GUARD(" + c + "_IS_ALG_VALID(alg), NULL, NULL);")
            definitions.append("\tUC_GUARD(ENG_IS_ENGINE_VALID(engine), NULL, NULL);")
            if c == "CIPHER":
                definitions.append("\tUC_GUARD(CIPHER_IS_MODE_VALID(mode), NULL, NULL);")
        definitions.append("\n\tif (UCLIB_ENGINE_INV == engine)\n\t\tengine = DEFAULT_" + c + "_ENGINE;")
        definitions.append("\n\tint i;")
        definitions.append("\tuint32_t mask = CFG_ALG_MASK(alg) + "
                           + ("CFG_MODE_MASK(mode) + " if c == "CIPHER" else "") + "CFG_ENG_MASK(engine);")
        c = c.lower()
        definitions.append(
            "\tfor (i=0; i<(ARRAY_SIZE(" + c + "_algs)); i++) {")
        definitions.append("\t\tUC_PASS(mask == " + c + "_algs[i].alg_id, " + c + "_algs[i].init_entry);")
        definitions.append("\t}\n\n\treturn NULL;\n}")
    cfile.write("\n".join(definitions))

    hfile.write("\n\n" + COMMENT_START_LINE + "\n" + "\t" * 8 +
                "DEFINITIONS AND TYPE DECLARATIONS\n" + COMMENT_END_LINE + "\n")
    hfile.write("\n".join(declarations))
    return SUCCESS


def gen_cfg_c_engine_entries(cfile, config, eng_entries):
    """ generate structs, engine entries array """

    # struct definitions that store supported algorithm id and fp to init
    # e.g.
    # typedef struct hash_alg_support_s
    # {
    #   uint32_t     alg_id;
    #   hash_init_fp init_entry;
    # }hash_alg_support_t;
    struct_defs = []
    cfile.write("\n")
    for c in config['supported-categories']:
        c = c.lower()
        struct_defs.append("\ntypedef struct " + c + "_alg_support_s")
        struct_defs.append("{\n\tuint32_t     alg_id;")
        struct_defs.append("\t" + c + "_init_fp init_entry;")
        struct_defs.append("}" + c + "_alg_support_t;")
    cfile.write("\n".join(struct_defs))

    # engine entries
    # e.g.
    # // SHA1 Engine Entries
    # int SHA1_CE_SW_init(hash_ctx_t *ctx);
    # int SHA1_CE_ARMV8_init(hash_ctx_t *ctx);
    entries_str = []
    e_arch = get_arch()
    eng_map = config['offtarget-engine-interface-vs-implementation-mapping']
    cfile.write("\n\n")

    # Workaround for TME-PBL pre-sil to unblock secboot validation
    riscv_wa = e_arch.lower().startswith("riscv")

    cats_new = [x for x in config['supported-categories']
                if x in list(eng_entries.keys())]

    for cat in cats_new:
        arg_str = '(' + g.ctx_type[cat] + ' *ctx);'
        ret_type = 'int '
        for algo in list(config[cat].keys()):
            if algo in ['AES', 'HC', 'DES', '2DES', 'TDES', 'SM4', 'STREAM', 'CHACHA20']:
                for mode in list(config[cat][algo].keys()):
                    entries_str.append("// " + algo + '-' + mode + " Engine Entries")
                    if algo in ['DES', '2DES', 'TDES', 'SM4', 'CHACHA20']:
                        eng_list = config[cat][algo][mode][e_arch]
                    else:  # AES and HC
                        eng_list = list(config[cat][algo][mode][e_arch].keys())
                    for e in eng_list:
                        if g.offtarget or (riscv_wa and "HASH" == cat):
                            e = eng_map[e]
                        if e in g.engines_supported and eng_entries[cat][algo][mode]['eng_entry'][e]:
                            entries_str.append(ret_type + eng_entries[cat][algo][mode]['eng_entry'][e] + arg_str)
                    entries_str.append("")    # for new line
            else:
                entries_str.append("// " + algo + " Engine Entries")
                for e in config[cat][algo][e_arch]:
                    if g.offtarget or (riscv_wa and "HASH" == cat):
                        e = eng_map[e]
                    if e in g.engines_supported and eng_entries[cat][algo]['eng_entry'][e]:
                        entries_str.append(ret_type + eng_entries[cat][algo]['eng_entry'][e] + arg_str)
                entries_str.append("")    # for new line
    # remove duplicates and write to file
    cfile.write("\n".join(remove_duplicates(entries_str)))

    # generate the arrays of supported algorithms along with their engine entries
    # e.g.
    # static const hash_alg_support_t hash_algs[] =
    # {
    #   /* SHA1 Engines */
    #   {
    #     (CFG_ALG_MASK(UCLIB_SHA1) + CFG_ENG_MASK(UCLIB_CE_SW)),
    #     SHA1_CE_SW_init,
    #   },
    #   {
    #     (CFG_ALG_MASK(UCLIB_SHA1) + CFG_ENG_MASK(UCLIB_CE_ARMV8)),
    #     SHA1_CE_SW_init,
    #   },
    alg_eng_map = []
    cfile.write("\n")

    for cat in cats_new:
        alg_eng_map.append("static const " + cat.lower() +
                           "_alg_support_t " + cat.lower() + "_algs[] =\n{")
        for algo in list(config[cat].keys()):
            if algo in ['AES', 'HC', 'STREAM']:
                for mode in list(config[cat][algo].keys()):
                    mode_c = config[cat][algo][mode]
                    mode_s = eng_entries[cat][algo][mode]
                    # add comment line
                    alg_eng_map.append("\n\t/* " + algo +
                                       "-" + mode + " Engines */")
                    if e_arch in list(mode_c.keys()):
                        for engine, bit_sizes in list(mode_c[e_arch].items()):
                            for s in bit_sizes["key-size"]:
                                entry_str = "\t{\n\t\t(CFG_ALG_MASK(" + mode_s['alg_mask'] + s + ")"
                                entry_str += " + CFG_MODE_MASK(" + mode_s['mode_mask'] + ")"
                                entry_str += " + CFG_ENG_MASK(UCLIB_CE_" + engine + ")),\n"
                                if g.offtarget or (riscv_wa and "HASH" == cat):
                                    engine_offtgt = eng_map[engine]
                                    entry_str += "\t\t" + mode_s['eng_entry'][engine_offtgt] + ',\n\t},'
                                    if engine_offtgt in g.engines_supported and mode_s['eng_entry'][engine_offtgt]:
                                        alg_eng_map.append(entry_str)
                                else:
                                    entry_str += "\t\t" + mode_s['eng_entry'][engine] + ',\n\t},'
                                    if engine in g.engines_supported and mode_s['eng_entry'][engine]:
                                        alg_eng_map.append(entry_str)
            elif algo in ['DES', '2DES', 'TDES', 'SM4', 'CHACHA20']:
                for mode in list(config[cat][algo].keys()):
                    mode_c = config[cat][algo][mode]
                    mode_s = eng_entries[cat][algo][mode]
                    # add comment line
                    alg_eng_map.append("\n\t/* " + algo +
                                       "-" + mode + " Engines */")
                    if e_arch in list(mode_c.keys()):
                        for engine in mode_c[e_arch]:
                            entry_str = "\t{\n\t\t(CFG_ALG_MASK(" + mode_s['alg_mask'] + ")"
                            entry_str += " + CFG_MODE_MASK(" + mode_s['mode_mask'] + ")"
                            entry_str += " + CFG_ENG_MASK(UCLIB_CE_" + engine + ")),\n"
                            if g.offtarget or (riscv_wa and "HASH" == cat):
                                engine = eng_map[engine]
                            entry_str += "\t\t" + mode_s['eng_entry'][engine] + ',\n\t},'
                            if engine in g.engines_supported and mode_s['eng_entry'][engine]:
                                alg_eng_map.append(entry_str)
            else:
                # add comment line
                alg_eng_map.append("\n\t/* " + algo + " Engines */")
                if e_arch in list(config[cat][algo].keys()):
                    for engine in config[cat][algo][e_arch]:
                        entry_str = "\t{\n\t\t(CFG_ALG_MASK(" + eng_entries[cat][algo]['alg_mask'] + ")"
                        entry_str += " + CFG_ENG_MASK(UCLIB_CE_" + engine + ")),\n"
                        if g.offtarget or (riscv_wa and "HASH" == cat):
                            engine = eng_map[engine]
                        entry_str += "\t\t" + eng_entries[cat][algo]['eng_entry'][engine] + ',\n\t},'
                        if engine in g.engines_supported and eng_entries[cat][algo]['eng_entry'][engine]:
                            alg_eng_map.append(entry_str)
        alg_eng_map.append("};\n")

    cfile.write("\n".join(alg_eng_map))
    return SUCCESS


def gen_cfg_c_macros(cfile, config, deps):
    """ Generate macro definitions in c file """
    macros = []
    cfile.write("\n\n" + COMMENT_START_LINE + "\n" + "\t" * 8 +
                "DEFINITIONS AND TYPE DECLARATIONS\n" + COMMENT_END_LINE + "\n")
    try:
        for m, v in deps["macros"].items():
            macros.append("#define " + m + "\t" * 4 + v)
        cfile.write("\n".join(macros))
        cfile.write("\n")
        for e, d in config["default-engines"].items():
            cfile.write("\n#define " + e + "\t" * 4 + d)
        cfile.write("\n")
        if g.offtarget:
            for p, c in config["default-prng-cb-offtarget"].items():
                cfile.write("\n#define " + p + "\t" * 4 + c)
        else:
            for p, c in config["default-prng-cb"].items():
                cfile.write("\n#define " + p + "\t" * 4 + c)
        return SUCCESS
    except KeyError:
        print("Invalid key")
        return ERROR


def gen_cfg_h_macros(hfile, deps):
    """ generate macro definitions in h file """
    cfg_p = []
    cfg_p.append("\n\n" + COMMENT_START_LINE + "\n" + "\t" * 8 +
                 "CONFIGURATION PARAMETERS\n" + COMMENT_END_LINE)

    if "rsa-config" in list(deps.keys()):
        cfg_p.append("/* RSA configuration parameters */")
        for rp, rp_v in deps["rsa-config"].items():
            cfg_p.append("#define " + rp + "\t" * 4 + rp_v)
        hfile.write("\n".join(cfg_p))
    else:
        print("RSA configuration parameters not provided in all_dependencies.json")
        return ERROR

    log_entry = []
    if g.debug_build:
        log_entry.append("\n\n" + COMMENT_START_LINE + "\n" + "\t" *
                         8 + "UCLIB DEBUG LOGGING ENABLED\n" + COMMENT_END_LINE)
    else:
        log_entry.append("\n\n" + COMMENT_START_LINE + "\n" + "\t" *
                         8 + "UCLIB DEBUG LOGGING DISABLED\n" + COMMENT_END_LINE)

        hfile.write("\n".join(log_entry))
    return SUCCESS


def gen_cfg_includes(hfile, cfile, deps):
    """ Add the list of required header files """
    inc = []
    comment_line = "\n\n" + COMMENT_START_LINE + "\n" + "\t" * \
        8 + "INCLUDE FILES FOR MODULE\n" + COMMENT_END_LINE + "\n"

    files = []
    if "include-files" in list(deps.keys()):
        files = deps["include-files"]
    else:
        print("Include files not provided in all_dependencies.json")
        return ERROR

    for f in files:
        inc.append("#include \"" + f + "\"")
    hfile.write(comment_line)
    hfile.write("\n".join(inc))

    cfile.write(comment_line)
    cfile.write("#include \"cfg.h\"")
    return SUCCESS


def gen_cfg_heading(hfile, cfile):
    """ Generate the comments at the top of files """
    heading = []
    try:
        hfile.write("#ifndef __UCLIB_PRV_CFG")
        hfile.write("\n#define __UCLIB_PRV_CFG\n")
        heading.append(COMMENT_START_LINE)
        heading.append("\n" + "\t" * 7 + "Unified Crypto Library - Internal Configuration APIs")
        heading.append("\nGENERAL DESCRIPTION" + "\n\nEXTERNALIZED FUNCTIONS")
        heading.append("\nINITIALIZATION AND SEQUENCING REQUIREMENTS")
        heading.append("\nCopyright (c) 2017 - 2020 Qualcomm Technologies, Inc. and/or its subsidiaries.")
        heading.append("All Rights Reserved.")
        heading.append("Confidential and Proprietary - Qualcomm Technologies, Inc.")
        heading.append(COMMENT_END_LINE + "\n\n" + "/*" + "*" * 80)
        heading.append(
            " * NOTE: This is an auto-generated file. Do not change the content manually")
        heading.append("*" * 80 + "*/")

        hfile.write("\n".join(heading))
        cfile.write("\n".join(heading))
        return SUCCESS
    except:
        return ERROR


def delete_files(files):
    """Delete any generated files if error occurs in the middle"""
    for f in files:
        if os.path.exists(f):
            print("deleting file %s", f)
            os.remove(f)
        else:
            print("file not found %s", f)


def remove_duplicates(arr):
    """ Remove duplicate entries from an arr (ignores newlines) """
    return [x for i, x in enumerate(arr) if not x.strip() or x not in arr[:i]]


def get_arch():
    """ Get the architecture string """
    if g.offtarget:
        return "x86_" + g.arch
    elif "dsp" == g.proc:
        return "dspq6_" + g.arch
    elif "arm" == g.proc:
        return "arm_" + g.arch
    elif "riscv" == g.proc:
        return "riscv_" + g.arch


def _parse_cmdline_args(args):
    """ Parse command line args """
    if len(args) != 4:
        raise Exception("Error: Wrong number of arguments.\
            \nSyntax: python config_generator.py <tgt_img> <chipset> <uclib_path>")
    tgt_image = args[1]
    chipset = args[2]
    uclib_path = os.path.abspath(args[3])

    return tgt_image, chipset, uclib_path

#-------------------------------------------------------------------------
# Use script in stanalone mode to autogenerate files for particular image
#-------------------------------------------------------------------------
if __name__ == "__main__":
    """
    Usage: python config_generator.py <tgt_img> <chipset> <uclib_path>
    """
    import sys

    tgt_image, chipset, uclib_path = _parse_cmdline_args(sys.argv)

    arch = ['32', '64']
    env = {
        'TARGET_DESTINATION': tgt_image,
        'CHIPSET': chipset,
        'UCLIB_PATH': uclib_path,
    }

    buildpath=os.path.join(uclib_path, 'uclib', 'build', tgt_image)
    if not os.path.isdir(buildpath):
        os.makedirs(buildpath)

    for ar in arch:
        generate_all_files(env, arch=ar, buildpath=buildpath)
