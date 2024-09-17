# MedicamentLibrary
 
### Архитектура проекта

Проект представляет собой распределенную систему, состоящую из множества сервисов, каждый из которых выполняет свою определенную задачу. Проект организован модульно, что позволяет легко добавлять новые компоненты и интеграции. Основная цель — создание платформы для работы с данными и сервисами, связанными с медикаментами, с возможностью расширения через API (gRPC, HTTP REST и т.п.).

#### Основные компоненты архитектуры:

- **`.ci`**: Директория, содержащая конфигурации для автоматизации процессов Continuous Integration (CI) и Continuous Deployment (CD). Здесь находятся скрипты и конфигурации для автоматического тестирования, сборки и развертывания проекта.
  
- **`common`**: Модуль общих библиотек и утилит, используемых в различных частях проекта. Здесь хранятся повторно используемые компоненты и вспомогательные функции.

- **`config`**: Директория для хранения конфигурационных файлов проекта, которые используются для управления настройками сервисов. Эти файлы могут различаться для разных окружений (например, разработка, тестирование, продакшн) и включать в себя параметры подключения к базам данных, API и т.д.

- **`dao`**: Data Access Objects (DAO) — слой, отвечающий за взаимодействие с базой данных. Каждый объект DAO представляет конкретное взаимодействие с таблицами в базе данных, обеспечивая изоляцию между бизнес-логикой и доступом к данным.

- **`data`**: Классы C++, описывающие структуры данных проекта. Эта директория включает в себя объекты, которые представляют данные, передаваемые между сервисами и хранимые в базе данных.

- **`dependencies`**: Директория для управления внешними зависимостями, которые не устанавливаются через системные пакетные менеджеры или требуются для проекта в виде сабмодулей.

- **`distribs`**: Содержит сабмодули для интеграции с внешними системами и базами данных (например, ClickHouse). Здесь находятся предкомпилированные и внешние модули, которые проект использует.

- **`docker`**: Контейнеризация проекта с помощью Docker. В этой директории хранятся `Dockerfile` для сборки образов и `docker-compose` файлы для одновременного запуска всех сервисов и зависимостей проекта.

- **`docs`**: Документация проекта, включая описание структуры, взаимодействий и API. Здесь также можно хранить диаграммы архитектуры и документацию для разработчиков.

- **`gateway`**: API Gateway для маршрутизации внешних запросов к сервисам. Это может быть слой для управления и авторизации запросов через HTTP REST или gRPC протоколы. Позволяет централизованно управлять внешними интерфейсами.

- **`logs`**: Система логирования для всех сервисов проекта. Здесь хранятся лог-файлы, а также конфигурации для логирования с различными уровнями детализации (info, debug, error).

- **`pipe`**: Обработка межсервисных взаимодействий. Это может быть система очередей сообщений (например, Kafka, RabbitMQ) или другие механизмы для асинхронного взаимодействия между сервисами.

- **`services`**: Внутренние сервисы системы, отвечающие за бизнес-логику проекта. Здесь реализуются основные функции проекта, которые могут быть интегрированы через разные интерфейсы взаимодействия (gRPC, REST и т.п.).

- **`tests`**: Тесты для проверки функциональности всех компонентов проекта. Включает юнит-тесты, интеграционные тесты и тесты производительности для каждого из модулей.

- **`tools`**: Вспомогательные утилиты для работы с проектом, такие как обновление сабмодулей или скрипты для автоматической установки зависимостей.

- **`installer`**: Скрипты установки, включая зависимости и конфигурации проекта. Используется для развёртывания окружений и настройки системы.

### Общее описание проекта

Проект "DrugLib" направлен на создание гибкой и модульной системы для работы с данными и сервисами, связанными с медициной и медикаментами. Система разработана с учетом использования современных технологий контейнеризации и модульной архитектуры, что позволяет легко масштабировать проект, добавлять новые зависимости и интеграции.

Ключевые особенности:
- **Модульность**: каждый сервис и модуль в системе разрабатывается как независимая единица, что позволяет легко интегрировать новые технологии (например, gRPC, REST) и обеспечивать независимое обновление компонентов.
- **Централизованное управление конфигурациями**: все конфигурационные файлы хранятся в одном месте, что упрощает управление окружениями и настройками для разных стадий разработки и продакшена.
- **Интеграция с внешними сервисами**: использование внешних модулей и баз данных (например, ClickHouse) для хранения и обработки данных, связанных с медицинскими препаратами.
- **Контейнеризация**: проект использует Docker для создания стандартных окружений, что упрощает развертывание и тестирование на разных платформах.
- **Межсервисное взаимодействие**: проект поддерживает различные схемы взаимодействия между сервисами (например, через очереди сообщений или API Gateway), что делает систему гибкой и адаптируемой под различные требования бизнеса.

Проект предназначен для использования в медицинских и фармацевтических компаниях, предоставляя удобные инструменты для работы с базами данных медикаментов и системами поиска информации на основе диагноза, пациента и других факторов.
