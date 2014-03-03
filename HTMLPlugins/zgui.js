var Z_WIDTH = 640;
var Z_HEIGHT = 480;
var Z_TEST = true;
$(document).ready(Z_ONLOAD);	 

function Z_GET_PARAM(url, name) {
    var match = RegExp('[?&]' + name + '=([^&]*)').exec(url);
    return match && decodeURIComponent(match[1].replace(/\+/g, ' '));
}    
    
function Z_ONLOAD(){
	if( window.location.toString().indexOf('without_settings') != -1 ){
		$('div.z_settings').hide();
        Z_WIDTH =  parseInt(Z_GET_PARAM(document.location, 'width'), 10) ;
        Z_HEIGHT = parseInt(Z_GET_PARAM(document.location, 'height'), 10) ;
		Z_TEST = false;
	}   
	Z_RESTORE();
    Z_RUN();        
}		
	
function Z_RUN(){		        
    var param = $.parseJSON(Z_PACK_PARAMS());
	Z_APPLY(param);	
}
	
function Z_PACK_PARAMS(){
	Z_SAVE();
	var param = {};
	var arr = $('#z_form').serializeArray();
	for(var i=0;i<arr.length;i++){
		var field = arr[i];
		param[field.name] = field.value;
	}	
	param.variant = Z_GET_VARIANT() || '';
	return JSON.stringify(param);
}

function Z_APPLY(param){	
	apply(param);
}

function Z_SAVE(){
	var arr = $('#z_form').serializeArray();	
	var jsn = JSON.stringify(arr);	
	localStorage.setItem(PLUGIN_NAME + '_settings', jsn);
}
	
function Z_RESTORE(){
	var jsn = localStorage.getItem(PLUGIN_NAME + '_settings');		
	var arr = $.parseJSON(jsn);
	$( '#z_form' ).deserialize(arr);
}

//////////////////////////////////
// variants
$(document).ready(function(){ $('.z_variant').click(z_set_select); });

function z_set_select(){	
	$('.z_variant').removeClass('z_select');
	$(this).addClass('z_select');
}

function Z_GET_VARIANT(){
	var v = $('.z_select').val();
	return v;
}

