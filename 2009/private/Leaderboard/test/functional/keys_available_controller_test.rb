require File.dirname(__FILE__) + '/../test_helper'
require 'keys_available_controller'

# Re-raise errors caught by the controller.
class KeysAvailableController; def rescue_action(e) raise e end; end

class KeysAvailableControllerTest < Test::Unit::TestCase
  def setup
    @controller = KeysAvailableController.new
    @request    = ActionController::TestRequest.new
    @response   = ActionController::TestResponse.new
  end

  # Replace this with your real tests.
  def test_truth
    assert true
  end
end
