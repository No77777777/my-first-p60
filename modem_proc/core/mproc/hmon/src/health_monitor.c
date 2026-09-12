#include "qmi_client.h"
#include "health_monitor_v01.h"
#include "msg.h"
#include "rcevt.h"
#include "rcinit.h"

#define HEALTH_MONITOR_CLIENT_SIG 0x1
#define HEALTH_MONITOR_EVENT_SIG  0x2

/* Probably later on move to dalconfig */
#define SELF_NAME "msm_mpss"

#define DEFAULT_TIMEOUT 5000

static rex_tcb_type *health_tcb = NULL;
static rex_crit_sect_type num_hc_cs;
static int num_hc = 0;

static void health_monitor_ind_cb
(
  qmi_client_type handle,
  unsigned int msg_id,
  void *ind_buf,
  unsigned int ind_buf_len,
  void *unused
)
{
  if(msg_id == QMI_HMON_HEALTH_CHECK_IND_V01)
  {
    rex_enter_crit_sect(&num_hc_cs);
    num_hc++;
    rex_leave_crit_sect(&num_hc_cs);
    rex_set_sigs(health_tcb, HEALTH_MONITOR_EVENT_SIG);
  }
}

void health_monitor_thread(void)
{
  qmi_client_error_type rc;
  qmi_client_type client;
  qmi_client_os_params os_params;
  hmon_register_req_msg_v01 *req;
  hmon_register_resp_msg_v01 resp;

  rcinit_handshake_startup();

  rex_init_crit_sect(&num_hc_cs);

  memset(&os_params, 0, sizeof(os_params));
  os_params.tcb = health_tcb = rex_self();
  os_params.sig = os_params.timer_sig = HEALTH_MONITOR_CLIENT_SIG;

  rc = qmi_client_init_instance(hmon_get_service_object_v01(),
                                QMI_CLIENT_INSTANCE_ANY,
                                health_monitor_ind_cb,
                                NULL,
                                &os_params,
                                0,
                                &client);
  if(rc != QMI_NO_ERR)
  {
    MSG_HIGH("Cannot create health monitor instance: rc = %d\n", rc, 0, 0);
    goto bail;
  }


  req = calloc(1, sizeof(*req));
  if(!req)
  {
    MSG_HIGH("Allocation failure!\n", 0,0,0);
    goto release_bail;
  }

  req->name_valid = 1;
  strlcpy(req->name, SELF_NAME, sizeof(req->name));
  req->timeout_valid = 1;
  req->timeout = DEFAULT_TIMEOUT;

  resp.resp.result = QMI_RESULT_SUCCESS_V01;
  resp.resp.error = QMI_ERR_NONE_V01;
  
  rc = qmi_client_send_msg_sync(client, QMI_HMON_REG_REQ_V01, req, sizeof(*req),
                                       &resp, sizeof(resp), 0);
  free(req);
  if(rc != QMI_NO_ERR || resp.resp.result != QMI_RESULT_SUCCESS_V01)
  {
    MSG_HIGH("register failed with rc = %d, result = %d,%d", rc, resp.resp.result, resp.resp.error);
    goto release_bail;
  }

  while(1)
  {
    rex_sigs_type sigs = rex_wait(HEALTH_MONITOR_EVENT_SIG);
    if(sigs & HEALTH_MONITOR_EVENT_SIG)
    {
      hmon_health_check_complete_req_msg_v01 comp_req;
      hmon_health_check_complete_resp_msg_v01 comp_resp;

      rex_clr_sigs(rex_self(), HEALTH_MONITOR_EVENT_SIG);

      rex_enter_crit_sect(&num_hc_cs);
      while(num_hc > 0)
      {
        num_hc--;
        rex_leave_crit_sect(&num_hc_cs);

        /* TODO Do an actual check of the system:
         * Ask the dog service? */

        memset(&comp_req, 0, sizeof(comp_req));
        comp_req.result_valid = 1;
        comp_req.result = HMON_CHECK_SUCCESS_V01;

        rc = qmi_client_send_msg_sync(client, QMI_HMON_HEALTH_CHECK_COMPLETE_REQ_V01,
            &comp_req, sizeof(comp_req), &comp_resp, sizeof(comp_resp), 0);
        if(rc != QMI_NO_ERR || comp_resp.resp.result != QMI_RESULT_SUCCESS_V01)
        {
          MSG_HIGH("Failure to send resp rc = %d, resp=%d,%d!\n",
              rc,comp_resp.resp.result,comp_resp.resp.error);
        }
        rex_enter_crit_sect(&num_hc_cs);
      }
      rex_leave_crit_sect(&num_hc_cs);
    }
  }
release_bail:
  qmi_client_release(client);
bail:
  rex_del_crit_sect(&num_hc_cs);
}
