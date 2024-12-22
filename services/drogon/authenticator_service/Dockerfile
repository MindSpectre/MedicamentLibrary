FROM ubuntu:latest

WORKDIR /app

COPY build-release/services/drogon/authenticator_service/ .

EXPOSE 8002
ENTRYPOINT ["./DrugLib_Services_Drogon_Authenticator", "--docker"]
