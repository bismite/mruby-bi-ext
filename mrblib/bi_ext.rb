
class Bi
  def self.add_action(node,action,callback)
    @@bi.add_action node, action, callback
  end
  def self.remove_action(action)
    @@bi.remove_action action
  end
end

class Bi::Action
  attr_accessor :callback
end

class Bi::Node
  def run_action(action,autoremove=true,callback_symbol=nil,&callback)
    if callback_symbol
      Bi::add_action self,action,callback_symbol
    elsif callback
      Bi::add_action self,action,callback
    elsif autoremove and not action.is_a? Bi::Action::Repeat
      Bi::add_action self,action,lambda{|node,action| node.remove_action action }
    else
      Bi::add_action self,action,nil
    end
    @actions ||= []
    @actions << action
    action.start self
  end
  def remove_action(action)
    if @actions and @actions.include? action
      Bi::remove_action action
      @actions.delete action
    end
  end
  def actions
    @actions
  end
  def remove_all_actions
    if @actions
      @actions.each{|a|
        Bi::remove_action a
      }
      @actions.clear
    end
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
