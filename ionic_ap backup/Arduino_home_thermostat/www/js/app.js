// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving Angular modules
// 'starter' is the name of this angular module example (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
var main = angular.module('starter', ['ionic'])
var arduinoServerUrl = 'http://74.59.2.195:8081';

main.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    if(window.cordova && window.cordova.plugins.Keyboard) {
      // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
      // for form inputs)
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);

      // Don't remove this line unless you know what you are doing. It stops the viewport
      // from snapping when text inputs are focused. Ionic handles this internally for
      // a much nicer keyboard experience.
      cordova.plugins.Keyboard.disableScroll(true);
    }
    if(window.StatusBar) {
      StatusBar.styleDefault();
    }
  });
})

main.config(function($stateProvider, $urlRouterProvider) {
        $stateProvider
            .state('tab', {
              url: "/tab",
              abstract: true,
              templateUrl: "#/tabs.html"
            })
            .state('tabs.dash', {
              url: '/dash',
              views: {
                'tab-dash':{
                  templateUrl: "#/tab-dash.htlm",
                  //controller: 'temperatureCtrl'
                }
              }
            })/*
            .state('tabs.programs', {
                url: '/programs',
                templateUrl: 'program.htm',
                controller: 'alarmCtrl'
            })*/

 
        $urlRouterProvider.otherwise('/tab/dash');
    });

main.controller("temperatureCtrl", ["$scope", "$interval", "ArduinoService", function($scope, $interval, service) {
  var autoRefresh;
  $scope.channels = [];

  $scope.options = {
  loop: false,
  //effect: 'fade',
  speed: 500,
  }
  $scope.data = {};
  $scope.$watch('data.slider', function(nv, ov) {
    $scope.slider = $scope.data.slider;
  })
    
  function startRefresh(){
    autoRefresh = $interval(function() {
    updateAjax();
    }, 5000);
  }


  function updateAjax() {
    service.getChannels(function(err, result) {//get json data
      if (err) {
        return alert(err);
      }
      // puis les mets dans le scope
      $scope.channels = result.channels;
    })
  };

  $scope.init = function() { //on load page first get data
    updateAjax();
    startRefresh()
  }


  $scope.switchChannel = function($scope, channel) { // change name function
    var switchCh = {canal : $scope.canal, status : $scope.status}
    service.switchChannel(switchCh, function() {
    });
    updateAjax();
  };

  $scope.channelsClk = function($scope, channel) {
    console.log("trig");
      var chanObj = {setPoint : $scope.setPoint, name : $scope.name, canal : $scope.canal
      };
      service.putChannels(chanObj, function() {
    });
  }

 $scope.stopRefresh = function() { //ng-mousedown
    $interval.cancel(autoRefresh);
  };

  $scope.restartRefresh = function() {
    startRefresh();
  };

$scope.$on('$destroy', function() {
    // Make sure that the interval is destroyed too
    $scope.stopRefresh();
  });
 
}]);



main.service('ArduinoService', ['$http', function($http) {
  return {
    getChannels: function(cb) {
      $http.get(arduinoServerUrl + '/ajax_inputs')
        .success(function(result) {
          cb(null, result);
        });
    },
    switchChannel: function(switchCh, cb) {
      $http.put(arduinoServerUrl + '/switch', {
          switchCh
        })
        .success(function(result) {
          cb(null, true);
        });
    },

    putChannels: function(channels, cb) {
      $http.put(arduinoServerUrl + '/channels', {
          channels
        })
        .success(function(result) {
          cb(null, true);
        });
    }
  };
}])

main.controller('alarmCtrl', [ "$scope", "$timeout", "alarmService", function($scope, $timeout, service){
     
      $scope.alarms = [];
      $scope.weekTypes = ["sunday", "monday", "tuesday", "wednesday", "thursday", "Friday", "saturday", "Week", "Week end", "All days"];

      function update(){
                 service.getAlarms(function(err, result) {
            if (err) {
              return alert(err);
            }
        // puis les mets dans le scope
            $scope.alarms = result.alarms;
          })
      };


    $scope.init = function() { //on load page first get data
      update();
    };      

    $scope.switchAlarm = function($scope, alarm) { // change name function
      var switchAlarm = {switchAlarm : alarm}
      service.switchAlarm(switchAlarm, function() {
      });
      update();     
    };

    $scope.alarmsClk = function($scope, index) {
        var chanObj = { index : index, data : $scope };
          service.putAlarms(chanObj, function() {
        });
          $timeout( function(){
            update()
          }, 2000);
    }
  }])

  main.service('alarmService', ['$http', function($http) {
      return {
         getAlarms: function(cb) {
            $http.get(arduinoServerUrl + '/ajax_alarms')
            .success(function(result) {
              cb(null, result);
          });
        },
      switchAlarm: function(switchAlarm, cb) {
        $http.put(arduinoServerUrl + '/switchAlarms', {
            switchAlarm
          })
          .success(function(result) {
            cb(null, true);
          });
      },

      putAlarms: function(alarms, cb) {
        $http.put(arduinoServerUrl + '/alarms', {
            alarms
          })
          .success(function(result) {
            cb(null, true);
          });
    }
  }
}]);






