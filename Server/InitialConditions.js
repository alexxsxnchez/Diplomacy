var Nation = require('./Nation.js');

var initialConditions = {
	territories: {
		'Kiel': Nation.GERMANY,
		'Berlin': Nation.GERMANY,
		'Munich': Nation.GERMANY,
		'Prussia': Nation.GERMANY,
		'Silesia': Nation.GERMANY,
		'Ruhr': Nation.GERMANY,
		'Warsaw': Nation.RUSSIA,
		'Livonia': Nation.RUSSIA,
		'Moscow': Nation.RUSSIA,
		'StPetersburg': Nation.RUSSIA,
		'Sevastopol': Nation.RUSSIA,
		'Ukraine': Nation.RUSSIA,
		'Ankara': Nation.TURKEY,
		'Smyrna': Nation.TURKEY,
		'Syria': Nation.TURKEY,
		'Armenia': Nation.TURKEY,
		'Constantinople': Nation.TURKEY,
		'Tyrolia': Nation.AUSTRIA,
		'Budapest': Nation.AUSTRIA,
		'Galicia': Nation.AUSTRIA,
		'Bohemia': Nation.AUSTRIA,
		'Vienna': Nation.AUSTRIA,
		'Trieste': Nation.AUSTRIA,
		'Venice': Nation.ITALY,
		'Apulia': Nation.ITALY,
		'Piedmont': Nation.ITALY,
		'Tuscany': Nation.ITALY,
		'Rome': Nation.ITALY,
		'Naples': Nation.ITALY,
		'Marseilles': Nation.FRANCE,
		'Burgundy': Nation.FRANCE,
		'Paris': Nation.FRANCE,
		'Gascony': Nation.FRANCE,
		'Picardy': Nation.FRANCE,
		'Brest': Nation.FRANCE,
		'London': Nation.ENGLAND,
		'Yorkshire': Nation.ENGLAND,
		'Wales': Nation.ENGLAND,
		'Liverpool': Nation.ENGLAND,
		'Clyde': Nation.ENGLAND,
		'Edinburgh': Nation.ENGLAND
	},
	units: {
		'Kiel': {
			type: 'fleet',
			nation: Nation.GERMANY
		},
		'Berlin': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'Munich': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'London': {
			type: 'fleet',
			nation: Nation.ENGLAND
		},
		'Liverpool': {
			type: 'army',
			nation: Nation.ENGLAND
		},
		'Edinburgh': {
			type: 'fleet',
			nation: Nation.ENGLAND
		},
		'Paris': {
			type: 'army',
			nation: Nation.FRANCE
		},
		'Marseilles': {
			type: 'army',
			nation: Nation.FRANCE
		},
		'Brest': {
			type: 'fleet',
			nation: Nation.FRANCE
		},
		'Venice': {
			type: 'army',
			nation: Nation.ITALY
		},
		'Rome': {
			type: 'army',
			nation: Nation.ITALY
		},
		'Naples': {
			type: 'fleet',
			nation: Nation.ITALY
		},
		'Trieste': {
			type: 'fleet',
			nation: Nation.AUSTRIA
		},
		'Vienna': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Budapest': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Constantinople': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Smyrna': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Ankara': {
			type: 'fleet',
			nation: Nation.TURKEY
		},
		'Sevastopol': {
			type: 'fleet',
			nation: Nation.RUSSIA
		},
		'Moscow': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Warsaw': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'StPetersburg': {
			type: 'fleet',
			nation: Nation.RUSSIA,
			coast: 'SC'
		}
	}
}

module.exports = initialConditions;
