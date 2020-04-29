$(function() {
      "use strict";
      
      var fs = require('fs');
      var gui = require('nw.gui');
      var clipboard = gui.Clipboard.get();
      
      function populateEmojiList() {
          fs.readFile('emoji.json', 'utf8', function(err, data) {
                          if (err) throw err;
                          // parse emoji.json
                          var obj = JSON.parse(data);
                          // sort by sort_order
                          obj.sort(function(a, b) {
                                       return a.sort_order - b.sort_order;
                                   });
                          // start pushing em to the list
                          for (var i = 0; i < obj.length; i++) {
                              var emoji = obj[i];
                              for (var j = 0; j < emoji.short_names.length; j++) {
                                  // add colons to the list of short_names on the object
                                  emoji.short_names[j] = ':' + emoji.short_names[j] + ':';
                              }
                              // start pushing em to the list
                              addEmoji(emoji);
                          }
                      });
      }
      
      function addEmoji(obj) {
          // no twitter emoji? get it out of here!
          if (!obj.has_img_twitter)
              return;
          // create button
          $('#'.concat(obj.category)).append('<div class="button emoji"></div>');
          // get a reference to the button we just created
          var emojiBtn = $('#'.concat(obj.category, ' .emoji:last-child'));
          emojiBtn.data('short_names', obj.short_names);
          emojiBtn.data('category', obj.category);
          // append image to button
          var emojiImg = '<img src="'.concat('./images/', obj.image, '" />');
          emojiBtn.append(emojiImg);
          
          setEventHandlers(obj, emojiBtn);
      }
      
      function setEventHandlers(obj, emojiBtn) {
          emojiBtn.mouseenter(function() {
                                  var placeholder = obj.short_names[0];
                                  for (var i = 1; i < obj.short_names.length; i++) {
                                      placeholder += ' ' + obj.short_names[i];
                                  }
                                  $('#emoji-id').attr('placeholder', placeholder);
                                  // update footer information
                                  $('#emoji-display img').attr('src', 'images/'.concat(obj.image));
                                  $('#emoji-display .name').text(obj.short_names[0].slice(1, -1));
                                  $('#emoji-display .short_name').text(placeholder);
                              });
          
          emojiBtn.click(function() {
                             var emoji = twemoji.convert.fromCodePoint(obj.unified);
                             alertify.dismissAll();
                             alertify.notify('Emoji successfully copied to clipboard', 'custom', 4, function() {
                                                 $('#emoji-info').removeClass('hide');
                                             });
                             $('#emoji-info').addClass('hide');
                             clipboard.set(String(emoji), 'text');
                         });
      }
      
      $(function() {
            populateEmojiList();
        });
  });