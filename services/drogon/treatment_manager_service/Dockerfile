
FROM ubuntu:latest

WORKDIR /app

COPY build-release/services/drogon/treatment_manager_service/ .

EXPOSE 8000
ENTRYPOINT ["./DrugLib_Services_Drogon_TreatmentManager", "--docker"]
