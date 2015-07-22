/// <reference path="../../../typings/jquery/jquery.d.ts"/>
/// <reference path="jquery/jquery-1.10.2.js"/>

//////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////
function initializeMeemabApp() {
	attachEventHandlers();

	setData("meemab-base-url", "http://meemab.cloudapp.net/api/");
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
			if (signIn()) {
				showPageContent('fragments/main.html');				
			}
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
	var url = getData("meemab-base-url") + resource;
	
	logLastRequest({ url: url, data: data });
	
	$.getJSON(url, data)
	.done(logLastResponse)
	.fail(logLastResponse)
	.always();
}


//////////////////////////////////////////////////////////////////////
// debug/trace/log
//////////////////////////////////////////////////////////////////////
function logInfo(msg) {
	var strmsg = JSON.stringify(msg);
	console.info(strmsg);
	$("#debug-log").after("<br>" + getFormattedTimestamp() + " " + strmsg);
} 

function logLastRequest(msg) {
	var strmsg = JSON.stringify(msg);
	console.info(strmsg);
	$("#debug-last-request").html(getFormattedTimestamp() + " " + strmsg);
} 

function logLastResponse(msg) {
	var strmsg = JSON.stringify(msg);
	console.info(strmsg);
	$("#debug-last-response").html(getFormattedTimestamp() + " " + strmsg);
} 

function getFormattedTimestamp() {
    var date = new Date();
    return date.getFullYear() + "-" + (date.getMonth() + 1) + "-" + date.getDate() + " " +  date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
}

//////////////////////////////////////////////////////////////////////
// Meemab helpers
//////////////////////////////////////////////////////////////////////
function signIn() {
	var accountName = queryAccountName();
	if (accountName == null || accountName == "") {
		return false;
	}
	
	createAccount(accountName);
	
	setData("account", accountName);
	updateSessionInfo();
	return true;
}

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

//////////////////////////////////////////////////////////////////////
// Meemab account management
//////////////////////////////////////////////////////////////////////
function createAccount(accountName) {
	// check if account exists
	logInfo("Creating account: " + accountName);
	getServerData("Account", { AccountName: accountName });

	// create if not exists
}


