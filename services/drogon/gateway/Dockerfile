FROM vcpkg_base:latest

WORKDIR /app

COPY build-release/services/drogon/gateway/ .

EXPOSE 3000
ENTRYPOINT ["./DrugLib_Services_Drogon_Gateway", "--docker"]
