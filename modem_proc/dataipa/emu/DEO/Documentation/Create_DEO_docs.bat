@rem **------------------------------------------------------------------------
@rem * create_core_docs.bat
@rem *
@rem * Batch file to run Doxygen, create documentation and collect warnings
@rem * 
@rem * Copyright (c) 2008, 2014 QUALCOMM Incorporated.
@rem * All Rights Reserved.
@rem * Qualcomm Confidential and Proprietary
@rem *-------------------------------------------------------------------------

@rem Create folders
@if not exist .\output mkdir .\output
@if not exist .\output\DEO mkdir .\output\DEO
@if not exist .\output\DEO\full_documentation mkdir .\output\DEO\full_documentation

@rem Run Doxygen and catch standard and error output to file
@"C:\Program Files\doxygen\bin\doxygen" .\Config\DEO_API_CONFIG 1> .\output\DEO\full_documentation\DEO_doxygen_runtime_output.txt 2>&1

@echo Done.

