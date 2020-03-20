#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <bi/ext/bi-ext.h>

// modules
void mrb_init_font(mrb_state *mrb, struct RClass *sg);
void mrb_init_label(mrb_state *mrb, struct RClass *sg);
void mrb_init_action(mrb_state *mrb, struct RClass *sg);


void mrb_mruby_bi_ext_gem_init(mrb_state* mrb)
{
  struct RClass *bi;
  bi = mrb_define_class(mrb, "Bi", mrb->object_class);
  MRB_SET_INSTANCE_TT(bi, MRB_TT_DATA);

  struct RClass *version = mrb_define_module_under(mrb,bi,"Version");
  mrb_define_const(mrb, version, "BI_EXT_MAJOR", mrb_fixnum_value(BI_EXT_MAJOR_VERSION));
  mrb_define_const(mrb, version, "BI_EXT_MINOR", mrb_fixnum_value(BI_EXT_MINOR_VERSION));
  mrb_define_const(mrb, version, "BI_EXT_PATCH", mrb_fixnum_value(BI_EXT_PATCHLEVEL));

#define DONE mrb_gc_arena_restore(mrb, 0)
  mrb_init_font(mrb,bi); DONE;
  mrb_init_label(mrb,bi); DONE;
  mrb_init_action(mrb,bi); DONE;
#undef DONE
}

void mrb_mruby_bi_ext_gem_final(mrb_state* mrb)
{
}
