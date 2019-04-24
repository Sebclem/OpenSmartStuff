pipeline {
    agent any
    options {
        buildDiscarder(logRotator(numToKeepStr: '5'))
    }


    stages {
        stage('Clone') {
            steps {
                echo env.BRANCH_NAME
                script {
                    if (env.BRANCH_NAME == 'master') {
                        git url: 'https://github.com/Sebclem/OpenSmartStuff.git', branch: 'master'
                    } else {
                        git url: 'https://github.com/Sebclem/OpenSmartStuff.git', branch: 'devel'
                    }
                }

            }
        }
        
    

        stage('Build Docker image') {
            steps {
                script {
                    app = docker.build("brokenfire/open-smart-stuff", '--rm=true .')
                }

            }
        }

        /* stage('Push Docker image') {
            
            steps {
                withCredentials([[$class: 'UsernamePasswordMultiBinding', credentialsId: 'dockerhub', usernameVariable: 'USERNAME', passwordVariable: 'PASSWORD']]) {
                    sh 'docker login -u $USERNAME -p $PASSWORD'
                    script {
                        if (env.BRANCH_NAME == 'master') {
                            app.push("master")
                        } else {
                            app.push("devel")
                        }
                    }
                }
            } 
        }*/


        stage('Cleaning') {
            steps {
                sh 'docker image prune -f'
            }
        }
        /* stage('Webhook') {
            steps {
                script {
                    sh "curl -X POST localhost:8555/"
                }

            }
        } */
    }
}