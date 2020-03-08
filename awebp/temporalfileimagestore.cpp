#include "wx/wxprec.h"
#include "temporalfileimagestore.h"
#include <wx/mstream.h>
#include "webp/encode.h"
#include "webp/decode.h"
#include "webp/mux.h"
#include "webp/demux.h"
#include <wx/imaggif.h>
