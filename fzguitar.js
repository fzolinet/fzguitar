/**
 * fzguitar.js
 */
if( typeof($j)=== "undefined" )
{
	var $j = jQuery;
}

$j(document).ready(function() {
	 $j("input[name='pitch_table']").click(function(){
		value = $j(this).val();
		$j.ajax({ 
			type: "GET",
			url: "pitch/record/load",
			data:{id: value},
			dataType: 'json',
			success:function(data){
				$j('input#edit-pitch-id').val(data.ID);
				$j('input#edit-pitch-descr').val(data.descr);
				$j('input#edit-pitch-pitch').val(data.pitch);
			},
			error: function(){
				alert("Ajax error");
			}
		});
	});
	 
	 $j("input[name='chords_table']").click(function(){
			value = $j(this).val();
			$j.ajax({ 
				type: "GET",
				url: "chords/record/load",
				data:{id: value},
				dataType: 'json',
				success:function(data){
					$j('input#edit-chords-id').val(data.ID);
					fzguitar_set_pitches(data.pitches, data.pitchID);
					$j('input#edit-chords-name').val(data.name);
					$j('input#edit-chords-tones').val(data.tones);
				},
				error: function(){
					alert("Ajax error");
				}
			});
		}); 
	 
	if( typeof(fzguitar_admin_settings) !== "undefined") 
	{
		$j("#color_rgb1").farbtastic("#edit-fzguitar-rgb1");
		$j("#color_rgb2").farbtastic("#edit-fzguitar-rgb2");
	}
});

function fzguitar_new_chord(){	
	$j.ajax({ 
		type: "GET",
		url: "chords/record/load",
		data:{id: -1},
		dataType: 'json',
		success:function(data){
			$j('input#edit-chords-id').val(data.ID);
			fzguitar_set_pitches(data.pitches, data.pitchID);
			$j('input#edit-chords-name').val(data.name);
			$j('input#edit-chords-tones').val(data.tones);
		},
		error: function(){
			alert("Ajax error");
		}
	});
}

function fzguitar_set_pitches(pitches, pitchID){
	$j('select#edit-chords-pitchid')[0].options.length=0;
	var ps = pitches.split("|");
	for (var i=0; i< ps.length; i++){
		if(pitchID == ps[i]){
			$j('select#edit-chords-pitchid').append("<option value='"+ps[i]+"' selected>"+ps[i]+"</option>");
		}else{
			$j('select#edit-chords-pitchid').append("<option value='"+ps[i]+"'>"+ps[i]+"</option>");
		}
	}
}

