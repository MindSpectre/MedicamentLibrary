#pragma once


namespace drug_lib::common::database
{
    class PqxxConnectParams
    {
    public:
        PqxxConnectParams(const std::string_view host,
                          const uint32_t port,
                          const std::string_view db_name,
                          const std::string_view login,
                          const std::string_view password): host_(host), port_(port), db_name_(db_name),
                                                            login_(login),
                                                            password_(password)
        {
        }

        PqxxConnectParams() = default;

        [[nodiscard]] const std::string_view& get_host() const
        {
            return host_;
        }

        void set_host(const std::string_view& host)
        {
            host_ = host;
        }

        [[nodiscard]] uint32_t get_port() const
        {
            return port_;
        }

        void set_port(const uint32_t port)
        {
            port_ = port;
        }

        [[nodiscard]] const std::string_view& get_db_name() const
        {
            return db_name_;
        }

        void set_db_name(const std::string_view& db_name)
        {
            db_name_ = db_name;
        }

        [[nodiscard]] const std::string_view& get_login() const
        {
            return login_;
        }

        void set_login(const std::string_view& login)
        {
            login_ = login;
        }

        [[nodiscard]] const std::string_view& get_password() const
        {
            return password_;
        }

        void set_password(const std::string_view& password)
        {
            password_ = password;
        }

        [[nodiscard]] std::string make_connect_string() const
        {
            std::ostringstream conn_str;
            conn_str << "host=" << host_
                << " port=" << port_
                << " user=" << login_
                << " password=" << password_
                << " dbname=" << db_name_;
            return conn_str.str();
        }

    private:
        std::string_view host_;
        uint32_t port_{};
        std::string_view db_name_;
        std::string_view login_;
        std::string_view password_;
    };
}
