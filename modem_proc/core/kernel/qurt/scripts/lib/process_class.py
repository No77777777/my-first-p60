import StringIO
import textwrap
import operator

def collect_process_classes(cfg):
    #
    #  In the future, this should generate an array of process classes by parsing XML.
    #  For now, we define most of the process information here.
    #

    #
    #  Give the C declaration of the QURTK_process_class structure.
    #

    struct_definition = """
        #include <stdint.h>

        struct _QURTK_process_class {
           char name[16];                       /* Zero-terminated string name of class */
           uint32_t namehash;                   /* 32-bit hash of class name */
           uint32_t capabilities;               /* Bitmask of capabilities given to this class */
           uint32_t examine_access;             /* Bitmask of which classes this class can examine */
           uint32_t modify_access;              /* Bitmask of which classes this class can modify */
           uint32_t spawn_access;               /* Bitmask of which classes this class can spawn */
           uint32_t kill_access;                /* Bitmask of which classes this class can kill */
           uint32_t migrate_access;             /* Bitmask of which classes this class can migrate to */
           uint16_t read_SID_access;            /* Bitmask of which SIDs this class can read */
           uint16_t write_SID_access;           /* Bitmask of which SIDs this class can write */
           uint16_t quota_threads;              /* Max number of threads a process of this class can have */
           uint16_t quota_vtlb_entries;         /* Max number of VTLB entries a process of this class can have */
           uint16_t quota_tlb_locks;            /* Max number of HW TLB entries a process of this class can lock */
           uint16_t quota_pages;                /* Max number of 4K pages of memory a process of this class can allocate */
           uint8_t class_number;                /* Small integer class number */
           uint8_t trap0_table_idx;             /* Index into QURTK_process_class_trap0tab */
        };

        typedef struct _QURTK_process_class QURTK_process_class;
    """

    #
    #  Give the list of process classes that are supported.
    #
    #  There is a current limit of 32 different capabilities.
    #
    #  In the future, these should be moved to being described in XML.
    #

    classes = """
        UKERNEL
        SRM
        SECURE
        CPZ
        ROOT
        SIGNED
        UNSIGNED
    """

    #
    #  Give the list of capabilities that are supported.
    #
    #  For now, define these.  Not all are used, and some may be
    #   modified or removed in the future.
    #
    #  There is a current limit of 32 different capabilities.
    #
    #  In the future, the list of capabilities probably can't be moved to XML easily;
    #   the reason is that the kernel code will expect certain capabilities, and it
    #   will already be compiled against certain capabilities.
    #

    capabilities = """
        VTLB
        SPAWN
        POWER
        FASTINT
        HVX
        SHUTDOWN
        UIMAGE
        ROOT_TRAP1
        SRM_TRAP1
        THREADS
    """

    #
    #   Define the trap0 table indexes of QURTK_process_class_trap0tab here. Use them
    #   here as well as while initializing QURTK_process_class_trap0tab so that they are
    #   hardcoded only at one place.
    #

    trap0_tab_indexes = """
        ROOT
        TRUSTED
        UNTRUSTED
    """

    capabilities = capabilities.split()
    classes = classes.split()
    indexes = trap0_tab_indexes.split()
    fo = StringIO.StringIO()

    print >>fo, textwrap.dedent(struct_definition).strip()

    print >>fo, '\nenum {'
    tmp = 0
    for idx, cap in enumerate(capabilities):
        tmp += (1 << idx)
        print >>fo, '   QURTK_CAP_%s = 0x%X,' % (cap, 1 << idx)
    print >>fo, '   QURTK_CAP_ALL = 0x%X,' % tmp
    print >>fo, '   QURTK_CAP_NONE = 0'
    print >>fo, '};\n'

    print >>fo, '\nenum {'
    for idx, name in enumerate(indexes):
        print >>fo, 'QURTK_TRAP0_IDX_%s = %d,' %(name,idx)
    print >>fo, 'QURTK_TRAP0_IDX_SIZE = %d,' % (idx+1)
    print >>fo, '};\n'
    class_list = []
    traptab_list = []
    hashes = []

    def make_namehash(s):
        #
        #  Create the 32-bit hash for a name; we pass the 32-bit hash across the
        #   kernel API boundary so that we don't have to deal with strings.
        #
        #  The hash is computed by an algorithm like this...
        #    hash = 0
        #    for each successive ASCII value X in the string:
        #      hash = (hash*0x103 + X), truncated to 32 bits
        #

        hash = 0
        for X in [ord(c) for c in str(s)]:
            hash = (hash*0x103 + X) & 0xFFFFFFFF

        return hash

    def get_name_index(s):
        #
        #  Look up the name s in the classes list.
        #  Return its index within the list or return -1
        #   if not present.
        #
        if s in classes:
            return classes.index(s)
        return -1

    def class_init(name,
                   capabilities,
                   examine_access,
                   modify_access,
                   spawn_access,
                   kill_access,
                   migrate_access,
                   trap0_table_idx):
        class_idx = classes.index(name)
        namehash = make_namehash(name)
        class_list.append(class_idx)
        if namehash in hashes:
            raise Exception('Class names have a hash collision...  Either change name or change hash algorithm')
        hashes.append(namehash)
        print >>fo, '   [%d] = {' % class_idx
        print >>fo, '      .name = "%s",' % name
        print >>fo, '      .namehash = 0x%08Xu,' % namehash
        print >>fo, '      .capabilities = %s,' % '|'.join(['QURTK_CAP_'+s for s in capabilities])
        for M in 'examine_access modify_access spawn_access kill_access migrate_access'.split():
            L = locals()[M]
            if 'ALL' in L:
                L = classes
            X = reduce(operator.or_, [0]+[1 << classes.index(E) for E in L])
            print >>fo, '      .%s = 0x%X,' % (M, X)
        print >>fo, '      .trap0_table_idx = %s,' % trap0_table_idx
        print >>fo, '      .read_SID_access = 0xFFFF,'
        print >>fo, '      .write_SID_access = 0xFFFF,'
        print >>fo, '      .quota_threads = 0xFFFF,'
        print >>fo, '      .quota_vtlb_entries = 0xFFFF,'
        print >>fo, '      .quota_tlb_locks = 0xFFFF,'
        print >>fo, '      .quota_pages = 0xFFFF,'
        print >>fo, '      .class_number = %d' % class_idx
        print >>fo, '   },'

    print >>fo, '#ifdef GEN_CONFIG_HEADER'
    print >>fo, 'extern QURTK_process_class QURTK_process_classes[];'
    print >>fo, 'extern const uint8_t QURTK_process_class_count;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_UKERNEL;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_SRM;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_SECURE;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_CPZ;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_ROOT;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_SIGNED;'
    print >>fo, 'extern const int8_t QURTK_process_class_index_UNSIGNED;'

    print >>fo, '#else /* GEN_CONFIG_HEADER */'
    print >>fo, 'QURTK_process_class QURTK_process_classes[] = {'

    non_ukernel = [C for C in classes if C != 'UKERNEL']
    non_threads = [C for C in capabilities if C != 'THREADS']

    #
    #  UKERNEL class.  This has all capabilities.  This is really only
    #   intended to be used during boot, but might find some use during
    #   fatal error processing as well.
    #

    class_init('UKERNEL',                       # name
               ['ALL'],                         # capabilities
               ['ALL'],                         # examine_access
               ['ALL'],                         # modify_access
               ['ALL'],                         # spawn_access
               ['ALL'],                         # kill_access
               ['ALL'],                         # migrate_access
               'QURTK_TRAP0_IDX_ROOT')          # QURTK_root_traptab

    #
    #  SRM class.  This has full rights to examine and modify all
    #   process classes except for UKERNEL.
    #

    class_init('SRM',                           # name
               ['ALL'],                         # capabilities
               non_ukernel,                     # examine_access
               non_ukernel,                     # modify_access
               [],                              # spawn_access
               [],                              # kill_access
               [],                              # migrate_access
               'QURTK_TRAP0_IDX_ROOT')          # QURTK_root_traptab

    #
    #  SECURE class.  This has no special rights.
    #

    class_init('SECURE',                        # name
               ['SRM_TRAP1','HVX','THREADS',
                'VTLB'],                        # capabilities
               [],                              # examine_access
               [],                              # modify_access
               [],                              # spawn_access
               [],                              # kill_access
               [],                              # migrate_access
               'QURTK_TRAP0_IDX_ROOT')          # QURTK_root_traptab

    #
    #  CPZ class.  This has no special rights.
    #

    class_init('CPZ',                           # name
               ['ALL'],                         # capabilities
               [],                              # examine_access
               [],                              # modify_access
               [],                              # spawn_access
               [],                              # kill_access
               [],                              # migrate_access
               'QURTK_TRAP0_IDX_UNTRUSTED')     # QURTK_root_traptab

    #
    #  ROOT class.  This has rights to spawn, examine, modify, and kill
    #   processes of SIGNED or UNSIGNED class.  It also has rights
    #   to kill processes of CPZ class.
    #

    class_init('ROOT',                          # name
               ['ALL'],                     # capabilities
               ['SIGNED','UNSIGNED'],           # examine_access
               ['SIGNED','UNSIGNED'],           # modify_access
               ['SIGNED','UNSIGNED'],           # spawn_access
               ['SIGNED','UNSIGNED','CPZ'],     # kill_access
               [],                              # migrate_access
               'QURTK_TRAP0_IDX_ROOT')          # QURTK_root_traptab

    #
    #  SIGNED class.  This has no special rights, but it can
    #   migrate to CPZ class.
    #

    class_init('SIGNED',                        # name
               ['FASTINT','SHUTDOWN','UIMAGE',
               'ROOT_TRAP1','SRM_TRAP1','HVX'], # capabilities
               [],                              # examine_access
               [],                              # modify_access
               [],                              # spawn_access
               [],                              # kill_access
               ['CPZ'],                         # migrate_access
               'QURTK_TRAP0_IDX_TRUSTED')       # QURTK_user_signed_traptab

    #
    #  UNSIGNED class.  This has no special rights.
    #

    class_init('UNSIGNED',                      # name
               ['SRM_TRAP1','HVX','THREADS',
                'VTLB'],                        # capabilities
               [],                              # examine_access
               [],                              # modify_access
               [],                              # spawn_access
               [],                              # kill_access
               [],                              # migrate_access
               'QURTK_TRAP0_IDX_UNTRUSTED')     # QURTK_user_unsigned_traptab

    print >>fo, '};'

    print >>fo, 'const uint8_t QURTK_process_class_count = %u;' % len(class_list)

    #
    # These process class variables should always be defined.
    # If you don't actually have a process class for one of these,
    #  the value can be -1.
    #

    print >>fo, 'const int8_t QURTK_process_class_index_UKERNEL = %d;' % get_name_index('UKERNEL')
    print >>fo, 'const int8_t QURTK_process_class_index_SRM = %d;' % get_name_index('SRM')
    print >>fo, 'const int8_t QURTK_process_class_index_SECURE = %d;' % get_name_index('SECURE')
    print >>fo, 'const int8_t QURTK_process_class_index_CPZ = %d;' % get_name_index('CPZ')
    print >>fo, 'const int8_t QURTK_process_class_index_ROOT = %d;' % get_name_index('ROOT')
    print >>fo, 'const int8_t QURTK_process_class_index_SIGNED = %d;' % get_name_index('SIGNED')
    print >>fo, 'const int8_t QURTK_process_class_index_UNSIGNED = %d;' % get_name_index('UNSIGNED')

    print >>fo, '#endif /* GEN_CONFIG_HEADER */'

    s = fo.getvalue()                           # Get the string we just built

    #
    #  Hack to avoid using stdint.h; we should try to use stdint.h instead
    #   of this hack...  That can really only happen when we get rid of all
    #   of the QuRT type definitions for things like uint32_t and delegate
    #   them all to stdint.h.
    #

    s = s.replace('uint32_t', 'unsigned int')
    s = s.replace('uint16_t', 'unsigned short')
    s = s.replace('uint8_t', 'unsigned char')
    s = s.replace('int8_t', 'signed char')
    s = s.replace('#include', '// #include')

    return s

if __name__ == '__main__':
    cfg = dict()
    print collect_process_classes(cfg)

