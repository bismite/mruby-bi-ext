#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/variable.h>
#include <bi/util.h>
#include <bi/ext/action.h>
#include <stdlib.h>
#include "bi_ext_inner_macro.h"

// Bi::add_action
static mrb_value mrb_add_action(mrb_state *mrb, mrb_value self)
{
    mrb_value node_obj, action_obj;
    mrb_get_args(mrb, "oo", &node_obj, &action_obj);

    // TODO: check error
    BiContext *c = DATA_PTR(self);
    BiNode *node = DATA_PTR(node_obj);
    BiAction *action = DATA_PTR(action_obj);

    bi_add_action(c,node,action);

    return self;
}

//
// Bi::Action class
//
static struct mrb_data_type const mrb_action_data_type = { "Action", mrb_free };

static mrb_value mrb_action_initialize(mrb_state *mrb, mrb_value self)
{
    BiAction *action = DATA_PTR(self);
    if (action == NULL) {
      action = mrb_malloc(mrb, sizeof(BiAction));
      if (NULL == action) {
        mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
      }
    }

    DATA_PTR(self) = action;
    DATA_TYPE(self) = &mrb_action_data_type;

    return self;
}

static mrb_value mrb_action_start(mrb_state *mrb, mrb_value self)
{
    mrb_value node_obj;
    mrb_get_args(mrb, "o", &node_obj);

    // TODO: check error
    BiAction *action = DATA_PTR(self);
    BiNode *node = DATA_PTR(node_obj);

    bi_action_start(node,action,bi_get_now());

    return self;
}

//
// Actions
//

static mrb_value mrb_action_move_to(mrb_state *mrb, mrb_value self)
{
    mrb_float duration;
    mrb_int x,y;
    mrb_get_args(mrb, "fii", &duration, &x, &y);

    // TODO: check error
    BiAction *action = DATA_PTR(self);

    bi_action_move_to_init(action,duration,x,y);

    return self;
}

static mrb_value mrb_action_sequence(mrb_state *mrb, mrb_value self)
{
    mrb_value array;
    mrb_get_args(mrb, "A", &array);

    // TODO: check error
    BiAction *action = DATA_PTR(self);

    mrb_int len = RARRAY_LEN(array);
    BiAction *actions[len];

    for(int i=0;i<len;i++){
      mrb_value a = mrb_ary_ref(mrb,array,i);
      actions[i] = DATA_PTR(a);
    }

    bi_action_sequence_init(action, len, actions );

    // retain
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@sequenced_actions"), array);

    return self;
}

static mrb_value mrb_action_repeat(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "o", &obj);

    // TODO: check error
    BiAction *action = DATA_PTR(self);
    BiAction *target = DATA_PTR(obj);

    bi_action_repeat_init(action, target);

    // retain
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@repeating_action"), obj);

    return self;
}

void mrb_init_action(mrb_state *mrb, struct RClass *bi)
{
  struct RClass *action;
  action = mrb_define_class_under(mrb, bi, "Action", mrb->object_class);
  MRB_SET_INSTANCE_TT(action, MRB_TT_DATA);

  mrb_define_method(mrb, bi, "add_action", mrb_add_action, MRB_ARGS_REQ(2)); // node,action

  mrb_define_method(mrb, action, "initialize", mrb_action_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, action, "start", mrb_action_start, MRB_ARGS_REQ(1)); // node

  mrb_define_method(mrb, action, "move_to", mrb_action_move_to, MRB_ARGS_REQ(3)); // duration,x,y
  mrb_define_method(mrb, action, "sequence", mrb_action_sequence, MRB_ARGS_REQ(1)); // actions
  mrb_define_method(mrb, action, "repeat", mrb_action_repeat, MRB_ARGS_REQ(1)); // action
}
