require File.dirname(__FILE__) + '/../test_helper'
require 'phone_home_session_controller'

# Re-raise errors caught by the controller.
class PhoneHomeSessionController; def rescue_action(e) raise e end; end

class PhoneHomeSessionControllerTest < Test::Unit::TestCase
  def setup
    @controller = PhoneHomeSessionController.new
    @request    = ActionController::TestRequest.new
    @response   = ActionController::TestResponse.new
  end

  # Replace this with your real tests.
  def test_truth
    assert true
  end
end
