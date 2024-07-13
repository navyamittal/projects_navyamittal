# Aggie's House
# Project Setup Instructions

Follow these instructions to set up the project environment and get everything running locally.

## Prerequisites

Before you start, ensure you have the following installed:
- Git
- Node.js
- Python
- PostgreSQL

## Cloning the Repository

First, clone the repository to your local machine:
```bash
git clone <repository-url>
```

## Environment Setup

### Install Node.js and npm

Install Node.js version 21.7.3 and npm version 10.5.2. We recommend using nvm to manage versions. Follow the instructions [here](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm).

### Install Python

Install Python version 3.10.12 by following the steps available [here](https://kinsta.com/knowledgebase/install-python/).

### Set Up PostgreSQL

Install and configure PostgreSQL version 14:
```bash
sudo apt install postgresql-14
sudo service postgresql start
sudo -u postgres psql
postgres=# create database mydb;
postgres=# create user myuser with encrypted password 'mypass';
postgres=# grant all privileges on database mydb to myuser;
sudo service postgresql restart
```

### Create Python Virtual Environment

Create and activate a Python virtual environment within the project directory:
```bash
python -m venv venv
source venv/bin/activate
```

### Install Python Dependencies

Install the required Python packages:
```bash
pip install -r requirements.txt
```

## Frontend Setup

### Install Node Packages

Navigate to the frontend directory and install required node packages:
```bash
cd frontend
npm install
```

### Environment Variables

Create a `.env` file in the frontend directory and define the necessary environment variables:
```bash
touch.env
```

Add the following variables:
```bash
REACT_APP_OAUTH_CLIENT_ID=<your-oauth-client-id>
REACT_APP_CHAT_ENGINE_PROJECT_ID=<your-chat-engine-project-id>
```


Get the OAuth credentials following these [Google Cloud instructions](https://support.google.com/cloud/answer/6158849).

For the Chat Engine project ID, follow the instructions [here](https://chatengine.io/docs/react/v1/getting_started).

## AWS Configuration

Create an AWS-S3 bucket and an IAM user following these [instructions](https://docs.aws.amazon.com/AmazonS3/latest/userguide/example-walkthroughs-managing-access-example1.html).

## Set Up the Backend `.env` File

Navigate back to the root directory of the project and create another `.env` file:
```bash
cd ..
touch .env
```


Include the following variables:
```bash
FLASK_APP=run.py
FLASK_ENV=development
pguser=<your-pg-user>
pgpass=<your-pg-pass>
pghost=localhost
pgport=5432
pgdb=<your-pg-db>
OAUTH_CLIENT_ID=<your-oauth-client-id>
OAUTH_CLIENT_SECRET=<your-oauth-client-secret>
AWS_ACCESS_KEY=<your-aws-access-key>
AWS_SECRET_KEY=<your-aws-secret-key>
S3_BUCKET=<your-s3-bucket-name>
AWS_REGION=<your-aws-region>
CHAT_ENGINE_PRIVATE_KEY=<your-chat-engine-private-key>
```


## Build and Run

Navigate back to the frontend directory to build the frontend:
```bash
cd frontend
npm run build
```

Then, navigate back to the root directory and start the application:
```bash
cd ..
flask run
```

You should now be able to access the project via [http://localhost:5000](http://localhost:5000).




















