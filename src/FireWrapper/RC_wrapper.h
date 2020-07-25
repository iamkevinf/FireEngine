#pragma once

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

#if defined _WIN32 || defined __CYGWIN__
#define API __declspec(dllexport)
#ifndef __GNUC__
#define snprintf sprintf_s
#endif
#else
#ifdef __GNUC__
#define API  __attribute__((__visibility__("default")))
#else
#define API
#endif
#endif

#if defined __cplusplus
#define EXTERN extern "C"
#else
#include <stdarg.h>
#include <stdbool.h>
#define EXTERN extern
#endif

#define RC_API EXTERN API

RC_API bgfx::TextureHandle rcLoadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);
