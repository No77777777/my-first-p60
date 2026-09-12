/*!
  @file qtrace_util_autogen.h
  @brief Auto-generated file. DO NOT EDIT.
*/

#ifndef QTRACE_UTIL_AUTOGEN_H
#define QTRACE_UTIL_AUTOGEN_H

/* Per-client log macros */

#if defined(FEATURE_QTRACE_DISABLE_DSM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_DSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HEAP) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HEAP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_CRASH) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CRASH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_QURT) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_QURT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_IMAGE) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_IMAGE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MPOWER) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MPOWER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_A2) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_A2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LFW) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LML1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LMAC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LRLC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LPDCP) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LPDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LRRC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_WFW) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_WL1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_WMAC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_WRLC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_WRRC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TFW) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TL1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TRLC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TRLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TMAC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TRRC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GFW) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GSNDCP) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GSNDCP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GLLC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GLLC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GRLCUL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GRLCUL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GRLCDL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GRLCDL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GRR) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GRR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GMAC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_GL1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_GL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ONEXL1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ONEXL2) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ONEXL3) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXL3(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ONEXMSC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXMSC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ONEXFW) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ONEXFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HMC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HMC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HDEC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HDEC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HRX) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HRX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HTX) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HTX(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HSRCH) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HSRCH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_CM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_SD) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SD(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MMOC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MMOC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_PM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_PM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_REG) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_REG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_CNM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CNM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MN) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MN(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_SM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_DS3GPP) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_DS3GPP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_DS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_DS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_IPA) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_IPA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TRM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TRM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_TCXOMGR) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_TCXOMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_SLPC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SLPC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_VSTMR) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_VSTMR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_FWS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_FWS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MSGR) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MSGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HWIO) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HWIO(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_CFCM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CFCM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_SMS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MEDIA) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MEDIA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_USIM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_USIM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_UTLKIT) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UTLKIT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_UDRVRS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UDRVRS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_UPBM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UPBM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_UQMI) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_UQMI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_AP) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_AP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_HOST) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_HOST(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_EMBMS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_EMBMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_VOLTE) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_VOLTE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_RF) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_RF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_OTHER) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_OTHER(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ALL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ALL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_QSH) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_QSH(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_QRARB) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_QRARB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_CAFI) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CAFI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MPERF) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MPERF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_WMS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_WMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GRF) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GRF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GFW) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GFW(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GML1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GML1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GMAC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GMAC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GL2UL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GL2DL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_NR5GRRC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_NR5GRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_PLT) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_PLT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_IMS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_IMS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LOC_SM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_SM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LOC_ME) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_ME(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LOC_PE) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_PE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LOC_AON) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LOC_AON(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_BSI) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_BSI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LSM) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LSM(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_L2LB) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_L2LB(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_RTP) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_RTP(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ENL2DL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ENL2DL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ENL2UL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ENL2UL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_CMAPI) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_CMAPI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_LMTSMGR) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_LMTSMGR(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MCS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MCFG) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MCFG(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_V2XL1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_V2XL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MSF) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MSF(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_MACE) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_MACE(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_THERMAL) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_THERMAL(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_ENL1) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_ENL1(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_SSGCCS) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SSGCCS(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_SSGSCA) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_SSGSCA(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_V2XRRC) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_V2XRRC(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif
#if defined(FEATURE_QTRACE_DISABLE_V2XL2) || defined(FEATURE_QTRACE_DISABLE)
  #define QTRACEI_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_DUMMY(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#else
  #define QTRACEI_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_S_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
  #define QTRACEI_INT_S_QSH_CLT_V2XL2(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ...) \
    QTRACEI_INT_S(_buffer_context_id, _client, _level, _subs_id, _tag, _summary, ##__VA_ARGS__)
#endif

/* String table macros */

#define QTRACEI_STR_TBL_1(_var, _count, _a0) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  const char * _var[_count] = { \
    _var ## _0 \
  }
#define QTRACEI_STR_TBL_2(_var, _count, _a0, _a1) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1 \
  }
#define QTRACEI_STR_TBL_3(_var, _count, _a0, _a1, _a2) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2 \
  }
#define QTRACEI_STR_TBL_4(_var, _count, _a0, _a1, _a2, _a3) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3 \
  }
#define QTRACEI_STR_TBL_5(_var, _count, _a0, _a1, _a2, _a3, _a4) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4 \
  }
#define QTRACEI_STR_TBL_6(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5 \
  }
#define QTRACEI_STR_TBL_7(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6 \
  }
#define QTRACEI_STR_TBL_8(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7 \
  }
#define QTRACEI_STR_TBL_9(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8 \
  }
#define QTRACEI_STR_TBL_10(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9 \
  }
#define QTRACEI_STR_TBL_11(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10 \
  }
#define QTRACEI_STR_TBL_12(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  static const char QTRACEI_STR_ATTR _var ## _11[] = _a11; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10, \
    _var ## _11 \
  }
#define QTRACEI_STR_TBL_13(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  static const char QTRACEI_STR_ATTR _var ## _11[] = _a11; \
  static const char QTRACEI_STR_ATTR _var ## _12[] = _a12; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10, \
    _var ## _11, \
    _var ## _12 \
  }
#define QTRACEI_STR_TBL_14(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  static const char QTRACEI_STR_ATTR _var ## _11[] = _a11; \
  static const char QTRACEI_STR_ATTR _var ## _12[] = _a12; \
  static const char QTRACEI_STR_ATTR _var ## _13[] = _a13; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10, \
    _var ## _11, \
    _var ## _12, \
    _var ## _13 \
  }
#define QTRACEI_STR_TBL_15(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  static const char QTRACEI_STR_ATTR _var ## _11[] = _a11; \
  static const char QTRACEI_STR_ATTR _var ## _12[] = _a12; \
  static const char QTRACEI_STR_ATTR _var ## _13[] = _a13; \
  static const char QTRACEI_STR_ATTR _var ## _14[] = _a14; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10, \
    _var ## _11, \
    _var ## _12, \
    _var ## _13, \
    _var ## _14 \
  }
#define QTRACEI_STR_TBL_16(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  static const char QTRACEI_STR_ATTR _var ## _11[] = _a11; \
  static const char QTRACEI_STR_ATTR _var ## _12[] = _a12; \
  static const char QTRACEI_STR_ATTR _var ## _13[] = _a13; \
  static const char QTRACEI_STR_ATTR _var ## _14[] = _a14; \
  static const char QTRACEI_STR_ATTR _var ## _15[] = _a15; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10, \
    _var ## _11, \
    _var ## _12, \
    _var ## _13, \
    _var ## _14, \
    _var ## _15 \
  }
#define QTRACEI_STR_TBL_113(_var, _count, _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31, _a32, _a33, _a34, _a35, _a36, _a37, _a38, _a39, _a40, _a41, _a42, _a43, _a44, _a45, _a46, _a47, _a48, _a49, _a50, _a51, _a52, _a53, _a54, _a55, _a56, _a57, _a58, _a59, _a60, _a61, _a62, _a63, _a64, _a65, _a66, _a67, _a68, _a69, _a70, _a71, _a72, _a73, _a74, _a75, _a76, _a77, _a78, _a79, _a80, _a81, _a82, _a83, _a84, _a85, _a86, _a87, _a88, _a89, _a90, _a91, _a92, _a93, _a94, _a95, _a96, _a97, _a98, _a99, _a100, _a101, _a102, _a103, _a104, _a105, _a106, _a107, _a108, _a109, _a110, _a111, _a112) \
  static const char QTRACEI_STR_ATTR _var ## _0[] = _a0; \
  static const char QTRACEI_STR_ATTR _var ## _1[] = _a1; \
  static const char QTRACEI_STR_ATTR _var ## _2[] = _a2; \
  static const char QTRACEI_STR_ATTR _var ## _3[] = _a3; \
  static const char QTRACEI_STR_ATTR _var ## _4[] = _a4; \
  static const char QTRACEI_STR_ATTR _var ## _5[] = _a5; \
  static const char QTRACEI_STR_ATTR _var ## _6[] = _a6; \
  static const char QTRACEI_STR_ATTR _var ## _7[] = _a7; \
  static const char QTRACEI_STR_ATTR _var ## _8[] = _a8; \
  static const char QTRACEI_STR_ATTR _var ## _9[] = _a9; \
  static const char QTRACEI_STR_ATTR _var ## _10[] = _a10; \
  static const char QTRACEI_STR_ATTR _var ## _11[] = _a11; \
  static const char QTRACEI_STR_ATTR _var ## _12[] = _a12; \
  static const char QTRACEI_STR_ATTR _var ## _13[] = _a13; \
  static const char QTRACEI_STR_ATTR _var ## _14[] = _a14; \
  static const char QTRACEI_STR_ATTR _var ## _15[] = _a15; \
  static const char QTRACEI_STR_ATTR _var ## _16[] = _a16; \
  static const char QTRACEI_STR_ATTR _var ## _17[] = _a17; \
  static const char QTRACEI_STR_ATTR _var ## _18[] = _a18; \
  static const char QTRACEI_STR_ATTR _var ## _19[] = _a19; \
  static const char QTRACEI_STR_ATTR _var ## _20[] = _a20; \
  static const char QTRACEI_STR_ATTR _var ## _21[] = _a21; \
  static const char QTRACEI_STR_ATTR _var ## _22[] = _a22; \
  static const char QTRACEI_STR_ATTR _var ## _23[] = _a23; \
  static const char QTRACEI_STR_ATTR _var ## _24[] = _a24; \
  static const char QTRACEI_STR_ATTR _var ## _25[] = _a25; \
  static const char QTRACEI_STR_ATTR _var ## _26[] = _a26; \
  static const char QTRACEI_STR_ATTR _var ## _27[] = _a27; \
  static const char QTRACEI_STR_ATTR _var ## _28[] = _a28; \
  static const char QTRACEI_STR_ATTR _var ## _29[] = _a29; \
  static const char QTRACEI_STR_ATTR _var ## _30[] = _a30; \
  static const char QTRACEI_STR_ATTR _var ## _31[] = _a31; \
  static const char QTRACEI_STR_ATTR _var ## _32[] = _a32; \
  static const char QTRACEI_STR_ATTR _var ## _33[] = _a33; \
  static const char QTRACEI_STR_ATTR _var ## _34[] = _a34; \
  static const char QTRACEI_STR_ATTR _var ## _35[] = _a35; \
  static const char QTRACEI_STR_ATTR _var ## _36[] = _a36; \
  static const char QTRACEI_STR_ATTR _var ## _37[] = _a37; \
  static const char QTRACEI_STR_ATTR _var ## _38[] = _a38; \
  static const char QTRACEI_STR_ATTR _var ## _39[] = _a39; \
  static const char QTRACEI_STR_ATTR _var ## _40[] = _a40; \
  static const char QTRACEI_STR_ATTR _var ## _41[] = _a41; \
  static const char QTRACEI_STR_ATTR _var ## _42[] = _a42; \
  static const char QTRACEI_STR_ATTR _var ## _43[] = _a43; \
  static const char QTRACEI_STR_ATTR _var ## _44[] = _a44; \
  static const char QTRACEI_STR_ATTR _var ## _45[] = _a45; \
  static const char QTRACEI_STR_ATTR _var ## _46[] = _a46; \
  static const char QTRACEI_STR_ATTR _var ## _47[] = _a47; \
  static const char QTRACEI_STR_ATTR _var ## _48[] = _a48; \
  static const char QTRACEI_STR_ATTR _var ## _49[] = _a49; \
  static const char QTRACEI_STR_ATTR _var ## _50[] = _a50; \
  static const char QTRACEI_STR_ATTR _var ## _51[] = _a51; \
  static const char QTRACEI_STR_ATTR _var ## _52[] = _a52; \
  static const char QTRACEI_STR_ATTR _var ## _53[] = _a53; \
  static const char QTRACEI_STR_ATTR _var ## _54[] = _a54; \
  static const char QTRACEI_STR_ATTR _var ## _55[] = _a55; \
  static const char QTRACEI_STR_ATTR _var ## _56[] = _a56; \
  static const char QTRACEI_STR_ATTR _var ## _57[] = _a57; \
  static const char QTRACEI_STR_ATTR _var ## _58[] = _a58; \
  static const char QTRACEI_STR_ATTR _var ## _59[] = _a59; \
  static const char QTRACEI_STR_ATTR _var ## _60[] = _a60; \
  static const char QTRACEI_STR_ATTR _var ## _61[] = _a61; \
  static const char QTRACEI_STR_ATTR _var ## _62[] = _a62; \
  static const char QTRACEI_STR_ATTR _var ## _63[] = _a63; \
  static const char QTRACEI_STR_ATTR _var ## _64[] = _a64; \
  static const char QTRACEI_STR_ATTR _var ## _65[] = _a65; \
  static const char QTRACEI_STR_ATTR _var ## _66[] = _a66; \
  static const char QTRACEI_STR_ATTR _var ## _67[] = _a67; \
  static const char QTRACEI_STR_ATTR _var ## _68[] = _a68; \
  static const char QTRACEI_STR_ATTR _var ## _69[] = _a69; \
  static const char QTRACEI_STR_ATTR _var ## _70[] = _a70; \
  static const char QTRACEI_STR_ATTR _var ## _71[] = _a71; \
  static const char QTRACEI_STR_ATTR _var ## _72[] = _a72; \
  static const char QTRACEI_STR_ATTR _var ## _73[] = _a73; \
  static const char QTRACEI_STR_ATTR _var ## _74[] = _a74; \
  static const char QTRACEI_STR_ATTR _var ## _75[] = _a75; \
  static const char QTRACEI_STR_ATTR _var ## _76[] = _a76; \
  static const char QTRACEI_STR_ATTR _var ## _77[] = _a77; \
  static const char QTRACEI_STR_ATTR _var ## _78[] = _a78; \
  static const char QTRACEI_STR_ATTR _var ## _79[] = _a79; \
  static const char QTRACEI_STR_ATTR _var ## _80[] = _a80; \
  static const char QTRACEI_STR_ATTR _var ## _81[] = _a81; \
  static const char QTRACEI_STR_ATTR _var ## _82[] = _a82; \
  static const char QTRACEI_STR_ATTR _var ## _83[] = _a83; \
  static const char QTRACEI_STR_ATTR _var ## _84[] = _a84; \
  static const char QTRACEI_STR_ATTR _var ## _85[] = _a85; \
  static const char QTRACEI_STR_ATTR _var ## _86[] = _a86; \
  static const char QTRACEI_STR_ATTR _var ## _87[] = _a87; \
  static const char QTRACEI_STR_ATTR _var ## _88[] = _a88; \
  static const char QTRACEI_STR_ATTR _var ## _89[] = _a89; \
  static const char QTRACEI_STR_ATTR _var ## _90[] = _a90; \
  static const char QTRACEI_STR_ATTR _var ## _91[] = _a91; \
  static const char QTRACEI_STR_ATTR _var ## _92[] = _a92; \
  static const char QTRACEI_STR_ATTR _var ## _93[] = _a93; \
  static const char QTRACEI_STR_ATTR _var ## _94[] = _a94; \
  static const char QTRACEI_STR_ATTR _var ## _95[] = _a95; \
  static const char QTRACEI_STR_ATTR _var ## _96[] = _a96; \
  static const char QTRACEI_STR_ATTR _var ## _97[] = _a97; \
  static const char QTRACEI_STR_ATTR _var ## _98[] = _a98; \
  static const char QTRACEI_STR_ATTR _var ## _99[] = _a99; \
  static const char QTRACEI_STR_ATTR _var ## _100[] = _a100; \
  static const char QTRACEI_STR_ATTR _var ## _101[] = _a101; \
  static const char QTRACEI_STR_ATTR _var ## _102[] = _a102; \
  static const char QTRACEI_STR_ATTR _var ## _103[] = _a103; \
  static const char QTRACEI_STR_ATTR _var ## _104[] = _a104; \
  static const char QTRACEI_STR_ATTR _var ## _105[] = _a105; \
  static const char QTRACEI_STR_ATTR _var ## _106[] = _a106; \
  static const char QTRACEI_STR_ATTR _var ## _107[] = _a107; \
  static const char QTRACEI_STR_ATTR _var ## _108[] = _a108; \
  static const char QTRACEI_STR_ATTR _var ## _109[] = _a109; \
  static const char QTRACEI_STR_ATTR _var ## _110[] = _a110; \
  static const char QTRACEI_STR_ATTR _var ## _111[] = _a111; \
  static const char QTRACEI_STR_ATTR _var ## _112[] = _a112; \
  const char * _var[_count] = { \
    _var ## _0, \
    _var ## _1, \
    _var ## _2, \
    _var ## _3, \
    _var ## _4, \
    _var ## _5, \
    _var ## _6, \
    _var ## _7, \
    _var ## _8, \
    _var ## _9, \
    _var ## _10, \
    _var ## _11, \
    _var ## _12, \
    _var ## _13, \
    _var ## _14, \
    _var ## _15, \
    _var ## _16, \
    _var ## _17, \
    _var ## _18, \
    _var ## _19, \
    _var ## _20, \
    _var ## _21, \
    _var ## _22, \
    _var ## _23, \
    _var ## _24, \
    _var ## _25, \
    _var ## _26, \
    _var ## _27, \
    _var ## _28, \
    _var ## _29, \
    _var ## _30, \
    _var ## _31, \
    _var ## _32, \
    _var ## _33, \
    _var ## _34, \
    _var ## _35, \
    _var ## _36, \
    _var ## _37, \
    _var ## _38, \
    _var ## _39, \
    _var ## _40, \
    _var ## _41, \
    _var ## _42, \
    _var ## _43, \
    _var ## _44, \
    _var ## _45, \
    _var ## _46, \
    _var ## _47, \
    _var ## _48, \
    _var ## _49, \
    _var ## _50, \
    _var ## _51, \
    _var ## _52, \
    _var ## _53, \
    _var ## _54, \
    _var ## _55, \
    _var ## _56, \
    _var ## _57, \
    _var ## _58, \
    _var ## _59, \
    _var ## _60, \
    _var ## _61, \
    _var ## _62, \
    _var ## _63, \
    _var ## _64, \
    _var ## _65, \
    _var ## _66, \
    _var ## _67, \
    _var ## _68, \
    _var ## _69, \
    _var ## _70, \
    _var ## _71, \
    _var ## _72, \
    _var ## _73, \
    _var ## _74, \
    _var ## _75, \
    _var ## _76, \
    _var ## _77, \
    _var ## _78, \
    _var ## _79, \
    _var ## _80, \
    _var ## _81, \
    _var ## _82, \
    _var ## _83, \
    _var ## _84, \
    _var ## _85, \
    _var ## _86, \
    _var ## _87, \
    _var ## _88, \
    _var ## _89, \
    _var ## _90, \
    _var ## _91, \
    _var ## _92, \
    _var ## _93, \
    _var ## _94, \
    _var ## _95, \
    _var ## _96, \
    _var ## _97, \
    _var ## _98, \
    _var ## _99, \
    _var ## _100, \
    _var ## _101, \
    _var ## _102, \
    _var ## _103, \
    _var ## _104, \
    _var ## _105, \
    _var ## _106, \
    _var ## _107, \
    _var ## _108, \
    _var ## _109, \
    _var ## _110, \
    _var ## _111, \
    _var ## _112 \
  }
#endif /* QTRACE_UTIL_AUTOGEN_H */