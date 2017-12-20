$(document).ready(function() {
    // Run the init method on document ready:
    page.loadAllEntries();
});

$("body").on("click", ".ReplyButton", function(){
    var arrTextArea = $(this).parent().find("textarea");
    var arrInput = $(this).parent().find("input");

    page.postReply(arrInput[0].value, arrTextArea[0].value);

    return false;
});

var page = {

    activity : {
        replyMessageActivity : false
    },

    loadAllEntries : function ()
    {
        $.eventGET('getAllEntries',function (r) {
            var events = [];

            for(var x = 0; x < r.length; x++) {
                events.push(page.render('event', r[x], (x === 0)))
            }

            $('#EventContainer').html(events.join(''));
        })
    },

    render : function(template, params, isTopEntry)
    {
        var arr = [];

        switch(template) {
            case 'event':
                timeString = params.time.day_name + ", " +
                             params.time.day_number + " " +
                             params.time.month_name + " " +
                             params.time.year + " (" +
                             params.time.hours + ":" +
                             params.time.minutes + ":" +
                             params.time.seconds + ")";

                arr = [
                        '<div class=', ((isTopEntry) ? '"TopEventElement"' : '"EventElement"'),'>',
                            '<span class="ElementHeader">',timeString,'</span>',
                            '<div class="ElementBody">',
                                '<div class="ElementInfo">',
                                    '<div>',
                                        '<span class="TextTitle">IP:</span>',
                                        '<span class="TextInfo">',params.device_ip,'</span>',
                                    '</div>',
                                    '<div>',
                                        '<span class="TextTitle">Agent:</span>',
                                        '<span class="TextInfo">',params.device_agent,'</span>',
                                    '</div>',
                                    '<div>',
                                        '<span class="TextTitle">UUID:</span>',
                                        '<span class="TextInfo">',params.device_uuid,'</span>',
                                    '</div>',
                                    '<br>',
                                    '<div>',
                                        '<span class="TextTitle">Reply:</span>',
                                        '<form method="post" action="">',
                                            '<textarea title="reply" class="TextArea" maxlength="50" rows="8" cols="16" autocomplete="off">',params.reply,'</textarea>',
                                            '<input type="hidden" name="rowID" autocomplete="off" value="',params.id,'"/>',
                                            '<input class="ReplyButton" type="submit" value="Send" />',
                                        '</form>',
                                    '</div>',
                                '</div>',
                                '<div class="ElementImage"><img class="Image" src="',params.base64Image,'" width="320" height="240"></div>',
                            '</div>',
                        '</div>'
                ];
                break;

            case 'oldEvent':
                arr = [
                        '<div class="EventElement">',
                        '<span class="ElementHeader">',params.created,'</span>',
                        '<img class="ElementImage" src="',params.base64Image,'" width="320" height="240">',
                        '</div>'
                ];
                break;
            default:
                break;
        }

        return arr.join('');
    },

    postReply : function(id, replyText)
    {
        var actionString = 'reply={"id": ' + id + ', "replyText": "' + replyText + '"}';

        $.eventPOST('submitReply',actionString, function(r) {
        });
    }
};

// Custom GET & POST wrappers:
$.eventPOST = function(action,data,callback)
{
    $.post('php/ajax.php?action='+action,data,callback,'json');
}

$.eventGET = function(action,data,callback)
{
    $.get('php/ajax.php?action='+action,data,callback,'json');
}