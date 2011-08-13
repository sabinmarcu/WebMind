jQuery(document).ready(function(){
    Map.lastid = 1;
    Map.container = jQuery("section");
    Map.node = function(parent)    {
         var _parent = parent;
         var _id = Map.lastid;
         Map.lastid++;
         function render()  {
              Map.container.append("
              <article><input type='text' id='elem"+this._id+"' name='elem"+this._id+"'>");
         }
     }
     Map.addNode = function()   {
         Map.nodes[Map.lastid] = new Map.node(0);
         Map.nodes[Map.lastid-1].render();
     }
     jQuery(".menui#new").click(function(){
         Map.addNode();
     })
})