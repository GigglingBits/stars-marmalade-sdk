/// <reference path="../../../typings/jquery/jquery.d.ts"/>
/// <reference path="jquery/jquery-1.10.2.js"/>

//////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////
function initializeMeemabApp() {
	attachEventHandlers();

	setData("meemab-base-url", "http://meemab.cloudapp.net/api");
}


//////////////////////////////////////////////////////////////////////
// DOM handling and navigation
//////////////////////////////////////////////////////////////////////
function attachEventHandlers() {
	$(function() {
		$("body").delegate(".meemabbutton", "click", function(){
			onButtonClicked(this.id);
		});
	});
}

function onButtonClicked(buttonid) {
	logInfo("Button clicked: " + buttonid);
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
	logInfo("Loaded new page content: " + filename);
}

function showMetricContent(buttonid) {
	var filename = 'fragments/metrics/' + buttonid + '.html';
	$('#contentpanel').load(filename);				
	logInfo("Loaded new metric content: " + filename);
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
// AJAX
//////////////////////////////////////////////////////////////////////
function getServerData(resource, data) {
	var request = {
		url: getData("meemab-base-url") + resource,
		type: "GET",
		dataType: "JSON"
	};
	
	logLastRequest(request);
	
	$.ajax(request)
	.done(logLastResponse)
	.fail(logLastResponse)
	.always();
}


//////////////////////////////////////////////////////////////////////
// debug/trace/log
//////////////////////////////////////////////////////////////////////
function logInfo(msg) {
	console.info(msg);
	$("#debug-log").before("<br>" + getFormattedTimestamp() + " " + msg);
} 

function logLastRequest(msg) {
	console.info(msg);
	$("#debug-last-request").html(getFormattedTimestamp() + " " + msg);
} 

function logLastResponse(msg) {
	console.info(msg);
	$("#debug-last-response").html(getFormattedTimestamp() + " " + msg);
} 

function getFormattedTimestamp() {
    var date = new Date();
    return date.getFullYear() + "-" + (date.getMonth() + 1) + "-" + date.getDate() + " " +  date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
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

