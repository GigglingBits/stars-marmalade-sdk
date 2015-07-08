/// <reference path="../../../typings/jquery/jquery.d.ts"/>
/// <reference path="jquery/jquery-1.10.2.js"/>

//////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// DOM handling and navigation
//////////////////////////////////////////////////////////////////////
function attachButtonHandler() {
	$(function() {
		$("body").delegate(".meemabbutton", "click", function(){
			onButtonClicked(this.id);
		});
	});
}

function onButtonClicked(buttonid) {
	console.info("Button clicked: " + buttonid);
	switch (buttonid) {
		case 'enter':
			showPageContent('fragments/logon.html');
			break;					
		case 'signup':
			var accountName = queryAccountName();
			setData("account", accountName);
			updateSessionInfo();
			showPageContent('fragments/main.html');
			break;	
		default:
			showMetricContent(buttonid);				
	}
}

function showPageContent(filename) {
	$('#page').load(filename);
	console.info("Loaded new page content: " + filename);
}

function showMetricContent(buttonid) {
	var filename = 'fragments/metrics/' + buttonid + '.html';
	$('#contentpanel').load(filename);				
	console.info("Loaded new metric content: " + filename);
}


//////////////////////////////////////////////////////////////////////
// State handling
//////////////////////////////////////////////////////////////////////
function getData(key) {
	return $(document).data(key);	
}

function setData(key, data) {
	return $(document).data(key, data);		
}

//////////////////////////////////////////////////////////////////////
// Meemab helpers
//////////////////////////////////////////////////////////////////////
function queryAccountName() {
	var person = '';
	while (person == null || person == "") {
		person = prompt('Please enter your account name', 'peter.pan@meemab.com');
	}
	return person;
}

function updateSessionInfo() {
	$('#sessioninfo').html('User: ' + getData("account"));
}

