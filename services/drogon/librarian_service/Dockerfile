FROM ubuntu:latest

WORKDIR /app

COPY build-release/services/drogon/librarian_service/ .

EXPOSE 8000
ENTRYPOINT ["./DrugLib_Services_Drogon_Librarian", "--docker"]
