jQuery(document).ready(function(){
    console.log(now);

    now.addNode = function(id, p)   {
	console.log("Received command to add node " + id + " with the parent " + p);
        Map.nodes[id] = new Map.node(id, p); 
        Map.nodes[id].render();
    } 
    now.initAddNode = function(id, p, x, y, txt, doc) {
	console.log("Adding node from memory " + id + " with parent " + p + ", coords : ("+ x + ", " + y + ") and text : " + txt);
        Map.nodes[id] = new Map.node(id, p);
        Map.nodes[id].x = x; 
        Map.nodes[id].y = y; 
        Map.nodes[id].doc = doc;
        Map.nodes[id].render();
	console.log( Map.nodes[id].elem.find("input").val() );
        Map.nodes[id].elem.find("input").val(txt);
        Map.nodes[id].elem.css("left", x+"px");
        Map.nodes[id].elem.css("top", y+"px");
        console.log(Map.nodes[id]);
    }
    now.letterPress = function(id, text, sender)   {
	if (sender == now.core.clientId) return;
	console.log("Updating text '" + text + "' for id " + id + " realtime data. ");
	console.log("Previous text : '" + Map.container.find("article#"+id+" input").val() + "'");
        Map.container.find("article#"+id+" input").val(text);
    }
    now.delNode = function (id) {
        del = Map.nodes[id];   
        p = Map.nodes[del.parent];  
        for(var i = 1; i <= del.cc; i++)   {   
            c = Map.nodes[del.children[i]];
            c.parent = del.parent;
            p.children[++p.cc] = c.id;
            Map.container.find("#raster"+c.id+"-"+id).remove();
        }
        p.children[id] = null; p.cc--;
        Map.container.find("#raster"+id+"-"+p.id).remove();
        Map.container.find("article#"+id).remove();
        Map.nodes[id] = null;
    }
    now.delBranch = function (id) {
        delbranchrec(id);
        p = Map.nodes[del.parent];
        p.children[del.id] = null;
        Map.container.find("#raster"+id+"-"+p.id).remove();       
        Map.container.find("article#"+id).remove();
        Map.nodes[id] = null;  
    }
    delbranchrec = function(id) {        
        del = Map.nodes[id];
        for (var i=1; i<=del.cc; i++) {
            c = Map.nodes[del.children[i]];
            if (c.children.length) {
                now.delBranch(c.id);
            } 
            Map.container.find("#raster"+c.id+"-"+id).remove();       
            Map.container.find("article#"+c.id).remove();
            Map.nodes[c.id] = null;
        } 
    }
    now.updateNode = function(id, text, x, y)  {
        e = Map.nodes[id];
        e.elem.find("input").val(text);
        e.x = x;
        e.y = y;
        e.elem.css("top", y).css("left", x);
        if (e.parent != 0){      
            parent = Map.nodes[e.parent];     
            if (!Map.container.find("#raster"+e.id+"-"+parent.id).length) {
                Map.container.append("<div class='raster' id='raster"+e.id+"-"+parent.id+"'></div>");
            }
            Map.container.find("#raster"+e.id+"-"+parent.id+" > *").remove();
            Map.container.find("#raster"+e.id+"-"+parent.id).drawLine(parent.x+75, parent.y+75, e.x+25, e.y+25);    
        }
        for(var i = 1; i <= e.cc; i++)   {   
            c = Map.nodes[e.children[i]];
            Map.container.find("#raster"+c.id+"-"+e.id+" > *").remove();  
            Map.container.find("#raster"+c.id+"-"+e.id).drawLine(c.x+75, c.y+75, e.x+25, e.y+25);  
        }
    }
    now.updateNodeText= function(id, text)    {
	console.log("Updating the text to : '" + text + "' for node" + id);
        e = Map.nodes[id];
        e.elem.find("input").val(text);
    }



    Map = new function()    { 

        this.container = jQuery("section");          
        this.nodes = [];            
        this.removeCont = function() {               
            Map.container.find("article#"+this.id).remove();

        }
        this.init = function()  {    
            this.container.html("");

            this.nodes = [];
            now.addNode(1, 0);
        }
        this.getDoc = function(id, title) {
            jQuery.get("doc.php?id="+id, function(data){
                Map.container.html(""); jQuery("#doctitle").html(title);
                for(i = 0; i < data.length; i++)  {
                    now.initAddNode(data[i].id, data[i].parent, data[i].x, data[i].y, data[i].text, data[i].doc)
                }  
            }, 'json')
        }
    }

    Map.node = function(id, p)    {

        this.parent = p;
        this.id = id;   
        this.cc = 0;
        this.children = [];
        this.doc = null;

        this.elem = null; 
        this.x = null;
        this.y = null;     

        if (p != 0)     {
            e = Map.nodes[p];
            parentcc = ++e.cc;
            e.children[parentcc] = this.id;                                          
        }

        this.render = function()  {
            var nclass = "common";
            if (this.parent == 0) nclass = "main";
            var link = "";
            if (this.parent != 0) link = "<a class='button' onclick='javascript:now.serverDelNode("+this.id+")'>-</a>";
            Map.container.append("<article id='"+this.id+"' class='"+nclass+"'><input type='text' onkeyup='javascript:now.serverLetterPress("+this.id+", this.value, "+now.core.clientId+")'  onchange='javascript:now.serverUpdateNodeText("+this.id+", this.value)' value='void node' /><div class='buttons'><a class='button' onclick='javascript:now.serverAddNode("+this.id+", "+this.doc+")'>+</a>"+link+"</div></buttons></article>")
            this.elem = Map.container.find("article#"+this.id);
            this.x = this.elem.css("left");   
            this.y = this.elem.css("top");   
            this.elem.draggable({
                stop:function(){       
                    var e = Map.nodes[this.id];   
                    var parent = Map.nodes[e.parent];     
                    e.x = parseInt(e.elem.css("left")); 
                    e.y = parseInt(e.elem.css("top"));      
                    now.serverUpdateNode(e.id, e.elem.find('input').val(), e.x, e.y);
                },
                start:function()    {
                    var e = Map.nodes[this.id];   
                    var parent = Map.nodes[e.parent];                
                    if (e.parent != 0){           
                        if (!Map.container.find("#raster"+e.id+"-"+parent.id).length) {
                            Map.container.append("<div class='raster' id='raster"+e.id+"-"+parent.id+"'></div>");
                        }
                        Map.container.find("#raster"+e.id+"-"+parent.id+" > *").remove();
                    }
                    for(var i = 1; i <= e.cc; i++)   {   
                        c = Map.nodes[e.children[i]];
                        Map.container.find("#raster"+c.id+"-"+e.id+" > *").remove();  
                    }
                }
            });
        }
    };   
    jQuery(".menui#new").click(function(){ 
        Map.container.html();
        Map.init();
    })         
    jQuery.get('doc.php', function(data) {
        console.log(data, data.length);
        e = jQuery('.menui#docs aside')
        e.html("");
        for(var i = 0; i < data.length; i++)    e.append("<li><input class='doclink' onclick='javascript:Map.getDoc("+data[i].id+", this.value)' value='"+data[i].title+"'/></li>")
    }, 'json');                 
    console.log(now.core.clientId);
})
