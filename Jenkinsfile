pipeline {
  agent any
  stages {
    stage('Setup') {
      steps {
        sh './configure'
      }
    }
    stage('Build') {
      steps {
        sh 'make'
      }
    }
    stage('Test') {
      steps {
        sh 'make test'
      }
    }
  }
}