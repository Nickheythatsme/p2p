pipeline {
  agent any
  stages {
    stage('Setup') {
      steps {
        cmakeBuild(buildDir: '.', buildType: 'Debug', installation: 'cmake3.15.2', cleanBuild: true)
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