/*
  Copyright (C) 2003 - 2009 GraphicsMagick Group
  Copyright (C) 2002 ImageMagick Studio
  Copyright 1991-1999 E. I. du Pont de Nemours and Company
 
  This program is covered by multiple licenses, which are described in
  Copyright.txt. You should have received a copy of Copyright.txt with this
  package; otherwise see http://www.graphicsmagick.org/www/Copyright.html.
 
  GraphicsMagick Application Programming Interface declarations.

*/

#if !defined(_MAGICK_API_H)
#define _MAGICK_API_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick_config.h"
#if defined(__cplusplus) || defined(c_plusplus)
#  undef inline
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/*
  Note that the WIN32 and WIN64 definitions are provided by the build
  configuration rather than the compiler.  Definitions available from
  the Windows compiler are _WIN32 and _WIN64.
*/
#if defined(WIN32) || defined(WIN64)
#  define MSWINDOWS
#endif /* defined(WIN32) || defined(WIN64) */

#if defined(MAGICK_IMPLEMENTATION)
#  if defined(MSWINDOWS)
  /* Use Visual C++ C inline method extension to improve performance */
#    if !defined(inline) && !defined(__cplusplus) && !defined(c_plusplus)
#      define inline __inline
#    endif
#  endif
#endif

#if defined(PREFIX_MAGICK_SYMBOLS)
#  include "magick/symbols.h"
#endif /* defined(PREFIX_MAGICK_SYMBOLS) */

#include "common.h"
#include "magick_types.h"
#include "analyze.h"
#include "attribute.h"
#include "average.h"
#include "blob.h"
#include "cdl.h"
#include "channel.h"
#include "color.h"
#include "color_lookup.h"
#include "colormap.h"
#include "command.h"
#include "compare.h"
#include "composite.h"
#include "compress.h"
#include "confirm_access.h"
#include "constitute.h"
#include "decorate.h"
#include "delegate.h"
#include "deprecate.h"
#include "describe.h"
#include "draw.h"
#include "effect.h"
#include "enhance.h"
#include "error.h"
#include "fx.h"
#include "gem.h"
#include "gradient.h"
#include "hclut.h"
#include "image.h"
#include "list.h"
#include "log.h"
#include "magic.h"
#include "magick.h"
#include "memory.h"
#include "module.h"
#include "monitor.h"
#include "montage.h"
#include "operator.h"
#include "paint.h"
#include "pixel_cache.h"
#include "pixel_iterator.h"
#include "plasma.h"
#include "profile.h"
#include "quantize.h"
#include "random.h"
#include "registry.h"
#include "render.h"
#include "resize.h"
#include "resource.h"
#include "shear.h"
#include "signature.h"
#include "statistics.h"
#include "texture.h"
#include "timer.h"
#include "transform.h"
#include "type.h"
#include "utility.h"
#include "version.h"

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif /* defined(__cplusplus) || defined(c_plusplus) */

#endif /* _MAGICK_API_H */
