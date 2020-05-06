
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

class Bi::Label
  attr_reader :text
  def text=(text)
    self.set_text text
  end
  alias :set_background_color :set_color
  alias :set_color :set_text_color
end

module Bi::Version
  def self.biext
    "#{BI_EXT_MAJOR}.#{BI_EXT_MINOR}.#{BI_EXT_PATCH}"
  end

  def self.mruby_biext
    "0.4.0"
  end
end
