#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <bi/node.h>
#include <bi/ext/font.h>
#include <stdlib.h>

// Bi::Label class
static void label_free(mrb_state *mrb, void *p)
{
  BiNode* node = (BiNode*)p;
  if (NULL != node) {
    for(int i=0;i<node->children_size;i++){
      free(node->children[i]);
    }
    mrb_free(mrb, node);
  }
}
static struct mrb_data_type const mrb_label_data_type = { "Label", label_free };


static mrb_value mrb_label_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value font_obj;
    mrb_get_args(mrb, "o", &font_obj);

    BiNode* node = DATA_PTR(self);
    if (node == NULL) {
      node = mrb_malloc(mrb, sizeof(BiNode));
      if (NULL == node) {
        mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
      }
    }

    bi_node_init(node);
    DATA_PTR(self) = node;
    DATA_TYPE(self) = &mrb_label_data_type;
    node->userdata = mrb_ptr(self);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@font"), font_obj);

    return self;
}

static mrb_value mrb_label_set_text(mrb_state *mrb, mrb_value self)
{
    mrb_value text;
    mrb_get_args(mrb, "S", &text );

    BiNode* n = DATA_PTR(self);

    // XXX: check error
    mrb_value font_obj = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@font"));
    BiFontAtlas* f = DATA_PTR(font_obj);

    bi_update_label(n, mrb_string_value_cstr(mrb,&text), f );

    return self;
}

void mrb_init_label(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *node;
  struct RClass *label;

  node = mrb_class_get_under(mrb, bi, "Node");
  label = mrb_define_class_under(mrb, bi, "Label", node);
  MRB_SET_INSTANCE_TT(label, MRB_TT_DATA);

  mrb_define_method(mrb, label, "initialize", mrb_label_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, label, "set_text", mrb_label_set_text, MRB_ARGS_REQ(1));
}
