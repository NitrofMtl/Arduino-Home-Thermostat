angular
    .module('app')


  .controller('configCtrl', [ "$scope", "$timeout", "configService", function($scope, $timeout, service){
     
      $scope.configs = [];


      function update(){
                 service.getConfigs(function(err, result) {
            if (err) {
              return alert(err);
            }
        // puis les mets dans le scope
            $scope.configs = result.configs[0];
          })
      };

   $scope.init = function() { //on load page first get data
      update();
    };      


    $scope.configClk = function(configs) {
        var chanObj = configs ;
        console.log(chanObj)
          service.putConfigs(chanObj, function() {
        });
          $timeout( function(){
            update()
          }, 2000);
    }
  }])

  .service('configService', ['$http', function($http) {
      return {
         getConfigs: function(cb) {
            $http.get('/configs')
            .success(function(result) {
              cb(null, result);
        });
      },

      putConfigs: function(config, cb) {
        $http.put('/configs', {
            config
          })
          .success(function(result) {
            cb(null, true);
          });
    }
  };
}])