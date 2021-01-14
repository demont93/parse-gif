#include "CommentExtension.h"
#include "ParseSubBlock.h"

void parsegif::ignore_comment_extension_block(BinaryFileBuf &buf)
{ ignore_sub_block(buf); }
