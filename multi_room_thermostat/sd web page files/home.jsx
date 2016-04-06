angular
  .module('app')

.controller('temperatureCtrl', ["$scope", "$interval", "ArduinoService", function($scope, $interval, service) {
  var autoRefresh;
  $scope.channels = [];

  function startRefresh(){
    autoRefresh = $interval(function() {
    updateAjax();
    }, 5000);
  }


  function updateAjax() {
    service.getChannels(function(err, result) {
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
}])

.service('ArduinoService', ['$http', function($http) {
  return {
    getChannels: function(cb) {
      $http.get('http://192.168.0.110/ajax_inputs')
        .success(function(result) {
          cb(null, result);
        });
    },
    switchChannel: function(switchCh, cb) {
      $http.put('http://192.168.0.110/switch', {
          switchCh
        })
        .success(function(result) {
          cb(null, true);
        });
    },

    putChannels: function(channels, cb) {
      $http.put('http://192.168.0.110/channels', {
          channels
        })
        .success(function(result) {
          cb(null, true);
        });
    }
  };

}])