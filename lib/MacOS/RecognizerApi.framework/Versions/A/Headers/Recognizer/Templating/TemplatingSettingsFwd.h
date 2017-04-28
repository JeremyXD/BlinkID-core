/**
 * @file TemplatingSettingsFwd.h
 *
 * Copyright (c)2017 Microblink Ltd. All rights reserved.
 *
 * ANY UNAUTHORIZED USE OR SALE, DUPLICATION, OR DISTRIBUTION
 * OF THIS PROGRAM OR ANY OF ITS PARTS, IN SOURCE OR BINARY FORMS,
 * WITH OR WITHOUT MODIFICATION, WITH THE PURPOSE OF ACQUIRING
 * UNLAWFUL MATERIAL OR ANY OTHER BENEFIT IS PROHIBITED!
 * THIS PROGRAM IS PROTECTED BY COPYRIGHT LAWS AND YOU MAY NOT
 * REVERSE ENGINEER, DECOMPILE, OR DISASSEMBLE IT.
 */
#ifndef TEMPLATINGSETTINGSFWD_H
#define TEMPLATINGSETTINGSFWD_H

#include "Recognizer/Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct TemplatingSettings
 * @brief Settings for configuring templating recognizers.
 */
struct TemplatingSettings;

/**
 * @brief typedef for TemplatingSettings structure
 */
MB_EXPORTED_TYPE typedef struct TemplatingSettings TemplatingSettings;

#ifdef __cplusplus
}
#endif

#endif
