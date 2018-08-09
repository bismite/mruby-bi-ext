#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

// modules
void mrb_init_font(mrb_state *mrb, struct RClass *sg);
void mrb_init_label(mrb_state *mrb, struct RClass *sg);
void mrb_init_action(mrb_state *mrb, struct RClass *sg);

void mrb_mruby_bi_ext_gem_init(mrb_state* mrb)
{
  struct RClass *bi;
  bi = mrb_define_class(mrb, "Bi", mrb->object_class);
  MRB_SET_INSTANCE_TT(bi, MRB_TT_DATA);

#define DONE mrb_gc_arena_restore(mrb, 0)
  mrb_init_font(mrb,bi); DONE;
  mrb_init_label(mrb,bi); DONE;
  mrb_init_action(mrb,bi); DONE;
#undef DONE
}

void mrb_mruby_bi_ext_gem_final(mrb_state* mrb)
{
}
