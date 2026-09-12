#pragma once
namespace q6zip
{
namespace executive
{
namespace overview
{
   /**
     @par Bootup
     @msc
     hscale="2.0";
     crt, qurt, rcinit_task, dlpager_task, ipa_zip_task, q6zip_sw, q6zip_ipa;
     ...;
     --- [ label="DL Pager created *BEFORE* rcinit_task" ];
     crt=>dlpager_task [ label="create" ];
     ...;
     dlpager_task=>q6zip_sw [ label="init" ];
     q6zip_sw=>dlpager_task [ label="initialized" ];
     dlpager_task=>q6zip_ipa [ label="init" ];
     q6zip_ipa=>ipa_zip_task [ label="register" ];
     dlpager_task=>qurt [ label="faulter = wait_for_page_fault" ];
     ...;
     --- [ label="DL Pager uses Q6ZIP_SW until Q6ZIP_IPA initializes" ];
     crt=>rcinit_task [ label="create" ];
     rcinit_task=>ipa_zip_task [ label="create" ];
     ipa_zip_task=>ipa_zip_task [ label="process_registration" ];
     ipa_zip_task>>q6zip_ipa [ label="registered" ];
     q6zip_ipa=>dlpager_task [ label="initialized" ];
     ...;
     --- [ label="DL Pager can switch to Q6ZIP_IPA" ];
     @endmsc
     */
   class bootup {};
}
}
namespace developer
{
namespace overview
{
/** 
   @par Initialization 
   @msc 
   hscale="2.0"; 
   dlpager_task,q6zip; 
   dlpager_task=>q6zip [ label="init( ro_dict, ro_dict_len, on_init, on_complete )"]; 
   q6zip>>dlpager_task [ label="initialized(TRUE)"]; dlpager_task=>dlpager_task [ 
   label="wait_for_page_fault" ]; 
   @endmsc 
 */
   class initialization {};

   /**
     @par Compression/decompression using submit
     @msc
     hscale="2.0";
     qurt,dlpager_task,ist_or_task,q6zip_iface,q6zip_request;
     dlpager_task=>qurt [ label="faulter = wait_for_page_fault()" ];
     dlpager_task=>dlpager_task [ label="pre_process( faulter )" ];
     --- [ label="pre-processing involves duplicate fault detection, swap pool mgmt"];
     ...;
     dlpager_task=>q6zip_iface [ label="get_request" ];
     --- [ label="if a request cannot be allocated, it is an error. Either SW is leaking requests or they are not tuned sufficiently" ];
     ...;
     dlpager_task=>dlpager_task [ label="prepare( request, faulter )" ];
     dlpager_task=>q6zip_iface [ label="submit( request )" ];
     dlpager_task=>qurt [ label="faulter = wait_for_page_fault" ];
     q6zip_iface>>ist_or_task [ label="on_complete( request, response )" ];
     --- [ label="on_complete() is invoked from an IST or task context; extract faulter information from request" ];
     ...;
     ist_or_task=>ist_or_task [ label="post_process( request, faulter )" ];
     ist_or_task=>q6zip_request [ label="free( &request )", url="\ref q6zip_request_free" ];
     ist_or_task=>qurt [ label="thread_resume( faulter )" ];
     @endmsc
     */
   class compress_or_decompress {};
}
}
}
