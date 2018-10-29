var view = new MapView();
var model = new MapModel(view);
var interactor = new Interactor(model);
var controller = new MapController(view, model);