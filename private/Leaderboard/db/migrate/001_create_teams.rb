class CreateTeams < ActiveRecord::Migration
  def self.up
    create_table "teams", :force => true do |t|
      t.column :title,                     :string
      t.column :login,                     :string
      t.column :url,                       :string
      t.column :crypted_password,          :string, :limit => 40
      t.column :salt,                      :string, :limit => 40
      t.column :created_at,                :datetime
      t.column :updated_at,                :datetime
      t.column :remember_token,            :string
      t.column :remember_token_expires_at, :datetime
    end
  end

  def self.down
    drop_table "teams"
  end
end
