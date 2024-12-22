FROM ubuntu:latest

WORKDIR /app

COPY build-release/services/drogon/authenticator/ .

EXPOSE 8002
ENTRYPOINT ["./DrugLib_Services_Drogon_Authenticator", "--docker"]
