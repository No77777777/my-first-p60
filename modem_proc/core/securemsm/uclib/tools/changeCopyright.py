"""
    Copyright (c) 2020 Qualcomm Technologies, Inc. and/or its subsidiaries.
    All Rights Reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.

    Description:
    ------------
    Script for changing the Copyright years in the specified patterns and paths.
    The paths and patterns can be specified in directory, find_pattern, replace_pattern.

    Usage:
    ------
    python changeCopyright.py
"""

import os, stat, fnmatch, datetime

directory = "../../uclib"
filePattern = "*"

now = datetime.datetime.now()
this_year = now.year
cur_yr_str = "{}".format(this_year)

last_year = now.year - 1
last_year_str = "{}".format(last_year)


find_pattern = ["Copyright (c) 2017-2019 Qualcomm ",
                "Copyright (c) 2019 Qualcomm ",
                "Copyright (c) 2018-2019 Qualcomm ",
                "Copyright (c) 2017 Qualcomm ",
                "Copyright (c) 2018 Qualcomm ",
                "Copyright 2019 QUALCOMM Inc",
                "Copyright (c) 2019 QUALCOMM Incorporated.",
                "Copyright (c) 2018 by QUALCOMM, Incorporated.  All Rights Reserved",
                "Copyright (c) 2019 by QUALCOMM, Incorporated.  All Rights Reserved",
                " - " + last_year_str]

replace_pattern = ["Copyright (c) 2017 - " + cur_yr_str + " Qualcomm ",
                   "Copyright (c) 2019 - " + cur_yr_str + " Qualcomm ",
                   "Copyright (c) 2018 - " + cur_yr_str + " Qualcomm ",
                   "Copyright (c) 2017 - " + cur_yr_str + " Qualcomm ",
                   "Copyright (c) 2018 - " + cur_yr_str + " Qualcomm ",
                   "Copyright (c) 2019 - " + cur_yr_str + " Qualcomm Technologies, Inc. and/or its subsidiaries",
                   "Copyright (c) 2019 - " + cur_yr_str + " Qualcomm Technologies, Inc. and/or its subsidiaries.",
                   "Copyright (c) 2018 - " + cur_yr_str + " Qualcomm Technologies, Inc. and/or its subsidiaries.\nAll Rights Reserved.\nConfidential and Proprietary - Qualcomm Technologies, Inc",
                   "Copyright (c) 2019 - " + cur_yr_str + " Qualcomm Technologies, Inc. and/or its subsidiaries.\nAll Rights Reserved.\nConfidential and Proprietary - Qualcomm Technologies, Inc",
                   " - " + cur_yr_str]


for path, dirs, files in os.walk(os.path.abspath(directory)):
    for filename in fnmatch.filter(files, filePattern):
        filepath = os.path.join(path, filename)

        if ".git" in filepath:
            continue
        if "changeCopyright.py" in filepath:
            continue

        with open(filepath) as f:
            s = f.read()
        for i in range(len(find_pattern)):
            s = s.replace(find_pattern[i], replace_pattern[i])
            st = os.stat(filepath)
            os.chmod(filepath, st.st_mode | stat.S_IWGRP | stat.S_IWUSR | stat.S_IWOTH )
            with open(filepath, "w") as f:
                f.write(s)
            os.chmod(filepath, st.st_mode)
