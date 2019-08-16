pipeline {
  agent any
  stages {
    stage('Setup') {
      steps {
        cmakeBuild(buildType: 'Debug', installation: 'cmake-lts', cleanBuild: true)
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