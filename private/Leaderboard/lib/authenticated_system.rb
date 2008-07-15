module AuthenticatedSystem
  protected
    # Returns true or false if the team is logged in.
    # Preloads @current_team with the team model if they're logged in.
    def team_logged_in?
      current_team != :false
    end
    
    # Accesses the current team from the session.  Set it to :false if login fails
    # so that future calls do not hit the database.
    def current_team
      @current_team ||= (login_from_session || login_from_basic_auth || login_from_cookie || :false)
    end
    
    # Store the given team in the session.
    def current_team=(new_team)
      session[:team] = (new_team.nil? || new_team.is_a?(Symbol)) ? nil : new_team.id
      @current_team = new_team
    end

    
    # Returns true or false if the admin is logged in.
    # Preloads @current_team with the admin model if they're logged in.
    def admin_logged_in?
      current_admin != :false
    end
    
    # Accesses the current admin from the session.  Set it to :false if login fails
    # so that future calls do not hit the database.
    def current_admin
      @current_admin ||= (login_admin_from_session || login_admin_from_basic_auth || login_admin_from_cookie || :false)
    end
    
    # Store the given admin in the session.
    def current_admin=(new_admin)
      session[:admin] = (new_admin.nil? || new_admin.is_a?(Symbol)) ? nil : new_admin.id
      @current_admin = new_admin
    end
    
    # Check if the team is authorized
    #
    # Override this method in your controllers if you want to restrict access
    # to only a few actions or if you want to check if the team
    # has the correct rights.
    #
    # Example:
    #
    #  # only allow nonbobs
    #  def authorized?
    #    current_team.login != "bob"
    #  end
    def team_authorized?
      team_logged_in?
    end

    # Filter method to enforce a login requirement.
    #
    # To require logins for all actions, use this in your controllers:
    #
    #   before_filter :login_required
    #
    # To require logins for specific actions, use this in your controllers:
    #
    #   before_filter :login_required, :only => [ :edit, :update ]
    #
    # To skip this in a subclassed controller:
    #
    #   skip_before_filter :login_required
    #
    def team_login_required
      team_authorized? || access_denied
    end
    

    
    # Check if the admin is authorized
    #
    # Override this method in your controllers if you want to restrict access
    # to only a few actions or if you want to check if the admin
    # has the correct rights.
    #
    # Example:
    #
    #  # only allow nonbobs
    #  def authorized?
    #    current_admin.login != "bob"
    #  end
    def admin_authorized?
      admin_logged_in?
    end

    # Filter method to enforce a login requirement.
    #
    # To require logins for all actions, use this in your controllers:
    #
    #   before_filter :login_required
    #
    # To require logins for specific actions, use this in your controllers:
    #
    #   before_filter :login_required, :only => [ :edit, :update ]
    #
    # To skip this in a subclassed controller:
    #
    #   skip_before_filter :login_required
    #
    def admin_login_required
      admin_authorized? || admin_access_denied
    end

    # Redirect as appropriate when an access request fails.
    #
    # The default action is to redirect to the login screen.
    #
    # Override this method in your controllers if you want to have special
    # behavior in case the team is not authorized
    # to access the requested action.  For example, a popup window might
    # simply close itself.
    def access_denied
      respond_to do |accepts|
        accepts.html do
          store_location
          redirect_to :controller => '/session', :action => 'new'
        end
        accepts.xml do
          headers["Status"]           = "Unauthorized"
          headers["WWW-Authenticate"] = %(Basic realm="Web Password")
          render :text => "Could't authenticate you", :status => '401 Unauthorized'
        end
      end
      false
    end
    

    
    # Redirect as appropriate when an access request fails.
    #
    # The default action is to redirect to the login screen.
    #
    # Override this method in your controllers if you want to have special
    # behavior in case the team is not authorized
    # to access the requested action.  For example, a popup window might
    # simply close itself.
    def admin_access_denied
      respond_to do |accepts|
        accepts.html do
          store_location
          redirect_to :controller => '/admin_session', :action => 'new'
        end
        accepts.xml do
          headers["Status"]           = "Unauthorized"
          headers["WWW-Authenticate"] = %(Basic realm="Web Password")
          render :text => "Could't authenticate you", :status => '401 Unauthorized'
        end
      end
      false
    end
    
    # Store the URI of the current request in the session.
    #
    # We can return to this location by calling #redirect_back_or_default.
    def store_location
      session[:return_to] = request.request_uri
    end
    
    # Redirect to the URI stored by the most recent store_location call or
    # to the passed default.
    def redirect_back_or_default(default)
      session[:return_to] ? redirect_to_url(session[:return_to]) : redirect_to(default)
      session[:return_to] = nil
    end
    
    # Inclusion hook to make #current_team and #logged_in?
    # available as ActionView helper methods.
    def self.included(base)
      base.send :helper_method, :current_team, :team_logged_in?, :current_admin, :admin_logged_in?
    end

    # Called from #current_user.  First attempt to login by the user id stored in the session.
    def login_from_session
      self.current_team = Team.find_by_id(session[:team]) if session[:team]
    end

    # Called from #current_user.  Now, attempt to login by basic authentication information.
    def login_from_basic_auth
      username, passwd = get_auth_data
      self.current_team = Team.authenticate(username, passwd) if username && passwd
    end

    # Called from #current_user.  Finaly, attempt to login by an expiring token in the cookie.
    def login_from_cookie      
      team = cookies[:auth_token] && Team.find_by_remember_token(cookies[:auth_token])
      if team && team.remember_token?
        team.remember_me
        cookies[:auth_token] = { :value => team.remember_token, :expires => team.remember_token_expires_at }
        self.current_team = team
      end
    end
    

    
    # Called from #current_user.  First attempt to login by the user id stored in the session.
    def login_admin_from_session
      self.current_admin = Admin.find_by_id(session[:admin]) if session[:admin]
    end

    # Called from #current_user.  Now, attempt to login by basic authentication information.
    def login_admin_from_basic_auth
      username, passwd = get_auth_data
      self.current_admin = Admin.authenticate(username, passwd) if username && passwd
    end

    # Called from #current_user.  Finaly, attempt to login by an expiring token in the cookie.
    def login_admin_from_cookie      
      admin = cookies[:admin_auth_token] && Admin.find_by_remember_token(cookies[:admin_auth_token])
      if admin && admin.remember_token?
        admin.remember_me
        cookies[:admin_auth_token] = { :value => admin.remember_token, :expires => admin.remember_token_expires_at }
        self.current_admin = admin
      end
    end

  private
    @@http_auth_headers = %w(X-HTTP_AUTHORIZATION HTTP_AUTHORIZATION Authorization)
    # gets BASIC auth info
    def get_auth_data
      auth_key  = @@http_auth_headers.detect { |h| request.env.has_key?(h) }
      auth_data = request.env[auth_key].to_s.split unless auth_key.blank?
      return auth_data && auth_data[0] == 'Basic' ? Base64.decode64(auth_data[1]).split(':')[0..1] : [nil, nil] 
    end
end
