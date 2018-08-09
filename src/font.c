#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <bi/font.h>
#include <bi/util.h>
#include <stdlib.h>
#include "bi_ext_inner_macro.h"

// Bi::Font class
static struct mrb_data_type const mrb_font_data_type = { "Font", mrb_free };

static mrb_value mrb_font_initialize(mrb_state *mrb, mrb_value self)
{
    // image, layout
    mrb_value img_obj, layout_name;
    mrb_get_args(mrb, "oS", &img_obj, &layout_name);

    BiFontAtlas *font = DATA_PTR(self);
    if (font == NULL) {
      font = mrb_malloc(mrb, sizeof(BiFontAtlas));
      if (NULL == font) {
        mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
      }
    }

    // TODO: error check
    BiTextureImage* img = DATA_PTR(img_obj);

    bi_load_font_layout( mrb_string_value_cstr(mrb,&layout_name), font );
    bi_set_color(font->color, 0xff,0xff,0xff,0xff);
    font->texture_image = img;

    DATA_PTR(self) = font;
    DATA_TYPE(self) = &mrb_font_data_type;

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@texture_image"), img_obj);

    return self;
}

_GET_(BiFontAtlas,font_size,bi_mrb_fixnum_value);

void mrb_init_font(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *font;
  font = mrb_define_class_under(mrb, bi, "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(font, MRB_TT_DATA);

  mrb_define_method(mrb, font, "initialize", mrb_font_initialize, MRB_ARGS_REQ(2)); // texture_image, layout_filename
  mrb_define_method(mrb, font, "size", mrb_BiFontAtlas_get_font_size, MRB_ARGS_NONE());

}
