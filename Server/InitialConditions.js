var Nation = require('./Nation.js');
var Phase = require('./Phase.js');

var initialConditions = {
	year: 1901,
	phase: Phase.SPRING,
	territories: {
		'Kiel': {
			nation: Nation.GERMANY,
			hasStar: true
		},
		'Berlin': {
			nation: Nation.GERMANY,
			hasStar: true
		},
		'Munich': {
			nation: Nation.GERMANY,
			hasStar: true
		},
		'Prussia': {
			nation: Nation.GERMANY,
			hasStar: false
		},
		'Silesia': {
			nation: Nation.GERMANY,
			hasStar: false
		},
		'Ruhr': {
			nation: Nation.GERMANY,
			hasStar: false
		},
		'Warsaw': {
			nation: Nation.RUSSIA,
			hasStar: true
		},
		'Livonia': {
			nation: Nation.RUSSIA,
			hasStar: false
		},
		'Moscow': {
			nation: Nation.RUSSIA,
			hasStar: true
		},
		'StPetersburg': {
			nation: Nation.RUSSIA,
			hasStar: true
		},
		'Sevastopol': {
			nation: Nation.RUSSIA,
			hasStar: true
		},
		'Ukraine': {
			nation: Nation.RUSSIA,
			hasStar: false
		},
		'Ankara': {
			nation: Nation.TURKEY,
			hasStar: true
		},
		'Smyrna': {
			nation: Nation.TURKEY,
			hasStar: true
		},
		'Syria': {
			nation: Nation.TURKEY,
			hasStar: false
		},
		'Armenia': {
			nation: Nation.TURKEY,
			hasStar: false
		},
		'Constantinople': {
			nation: Nation.TURKEY,
			hasStar: true
		},
		'Tyrolia': {
			nation: Nation.AUSTRIA,
			hasStar: false
		},
		'Budapest': {
			nation: Nation.AUSTRIA,
			hasStar: true
		},
		'Galicia': {
			nation: Nation.AUSTRIA,
			hasStar: false
		},
		'Bohemia': {
			nation: Nation.AUSTRIA,
			hasStar: false
		},
		'Vienna': {
			nation: Nation.AUSTRIA,
			hasStar: true
		},
		'Trieste': {
			nation: Nation.AUSTRIA,
			hasStar: true
		},
		'Venice': {
			nation: Nation.ITALY,
			hasStar: true
		},
		'Apulia': {
			nation: Nation.ITALY,
			hasStar: false
		},
		'Piedmont': {
			nation: Nation.ITALY,
			hasStar: false
		},
		'Tuscany': {
			nation: Nation.ITALY,
			hasStar: false
		},
		'Rome': {
			nation: Nation.ITALY,
			hasStar: true
		},
		'Naples': {
			nation: Nation.ITALY,
			hasStar: true
		},
		'Marseilles': {
			nation: Nation.FRANCE,
			hasStar: true
		},
		'Burgundy': {
			nation: Nation.FRANCE,
			hasStar: false
		},
		'Paris': {
			nation: Nation.FRANCE,
			hasStar: true
		},
		'Gascony': {
			nation: Nation.FRANCE,
			hasStar: false
		},
		'Picardy': {
			nation: Nation.FRANCE,
			hasStar: false
		},
		'Brest': {
			nation: Nation.FRANCE,
			hasStar: true
		},
		'London': {
			nation: Nation.ENGLAND,
			hasStar: true
		},
		'Yorkshire': {
			nation: Nation.ENGLAND,
			hasStar: false
		},
		'Wales': {
			nation: Nation.ENGLAND,
			hasStar: false
		},
		'Liverpool': {
			nation: Nation.ENGLAND,
			hasStar: true
		},
		'Clyde': {
			nation: Nation.ENGLAND,
			hasStar: false
		},
		'Edinburgh': {
			nation: Nation.ENGLAND,
			hasStar: true
		},
		'Norway': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Sweden': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Denmark': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Holland': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Belgium': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Spain': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Portugal': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Tunis': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Greece': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Serbia': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Rumania': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Bulgaria': {
			nation: Nation.NEUTRAL,
			hasStar: true
		},
		'Finland': {
			nation: Nation.NEUTRAL,
			hasStar: false
		},
		'Albania': {
			nation: Nation.NEUTRAL,
			hasStar: false
		},
		'NorthAfrica': {
			nation: Nation.NEUTRAL,
			hasStar: false
		}
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
