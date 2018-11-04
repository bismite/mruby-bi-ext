
class Bi::Node
  attr_reader :actions
  def add_action(action)
    @actions ||= []
    @actions << action
    self._add_action action
    action
  end
  def remove_action(action)
    @actions ||= []
    @actions.delete action
    self._remove_action action
    action
  end
  def remove_all_actions
    if @actions
      @actions.each{|a|
        self._remove_action a
      }
      @actions.clear
    end
  end
end

class Bi::Action::Base
  attr_accessor :callback
end
