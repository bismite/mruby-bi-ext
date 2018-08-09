
class Bi
  def self.add_action(node,action)
    @@bi.add_action node, action
  end
end

class Bi::Node
  def run_action(action)
    Bi::add_action self,action
    @actions ||= []
    @actions << action
    action.start self
  end
end

class Bi::Action::MoveTo < Bi::Action
  def initialize(duration,x,y)
    super
    self.move_to duration,x,y
  end
end

class Bi::Action::Sequence < Bi::Action
  def initialize(actions)
    super
    self.sequence actions
  end
end

class Bi::Action::Repeat < Bi::Action
  def initialize(action)
    super
    self.repeat action
  end
end
