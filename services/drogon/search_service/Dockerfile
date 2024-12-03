FROM vcpkg_base:latest

WORKDIR /app

COPY build/services/drogon/search_service/ .

EXPOSE 8001
ENTRYPOINT ["./DrugLib_Services_Drogon_Search", "--docker"]
