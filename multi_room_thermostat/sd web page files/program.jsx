angular
    .module('app')


	.controller('alarmCtrl', [ "$scope", "$timeout", "alarmService", function($scope, $timeout, service){
	   
	   	$scope.alarms = [];
	   	$scope.weekTypes = ["null", "sunday", "monday", "tuesday", "wednesday", "thursday", "Friday", "saturday", "Week", "Week end", "All days"];

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

	.service('alarmService', ['$http', function($http) {
  		return {
   			 getAlarms: function(cb) {
      			$http.get('http://192.168.0.110/ajax_alarms')
        		.success(function(result) {
          		cb(null, result);
        });
	    },
	    switchAlarm: function(switchAlarm, cb) {
	      $http.put('http://192.168.0.110/switchAlarms', {
	          switchAlarm
	        })
	        .success(function(result) {
	          cb(null, true);
	        });
	    },

	    putAlarms: function(alarms, cb) {
	      $http.put('http://192.168.0.110/alarms', {
	          alarms
	        })
	        .success(function(result) {
	          cb(null, true);
	        });
    }



  };

}])