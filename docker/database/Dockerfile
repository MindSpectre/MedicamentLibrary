FROM postgres:latest

# Install PostgreSQL contrib for extensions like pg_trgm
RUN apt-get update && apt-get install -y postgresql-contrib && rm -rf /var/lib/apt/lists/*
WORKDIR /app

# Expose PostgreSQL's default port
EXPOSE 5432
