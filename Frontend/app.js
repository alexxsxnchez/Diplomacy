var mapView = new MapView();
var gameView = new GameView();
var presenter = new Presenter(mapView, gameView);
var model = new MapModel(presenter);
var interactor = new Interactor(model);
var controller = new MapController(mapView, gameView, model);