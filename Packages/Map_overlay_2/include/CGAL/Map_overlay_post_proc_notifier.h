#ifndef CGAL_MAP_OVERLAY_POST_PROC_NOTIFIER_H
#define CGAL_MAP_OVERLAY_POST_PROC_NOTIFIER_H   

CGAL_BEGIN_NAMESPACE         

template <class Arrangement_>
class Map_overlay_post_proc_notifier :  public Arrangement_::Change_notification 
{
public:
  typedef Arrangement_                                                     Arrangement;
  typedef typename Arrangement::Vertex                                     Vertex;
  typedef typename Arrangement::Face                                       Face;
  typedef typename Arrangement::Halfedge                                   Halfedge;
  typedef typename Arrangement::Vertex_handle                              Vertex_handle;
  typedef typename Arrangement::Halfedge_handle                            Halfedge_handle;
  typedef typename Arrangement::Face_handle                                Face_handle;
  typedef typename Arrangement::Vertex_const_handle                        Vertex_const_handle;
  typedef typename Arrangement::Halfedge_const_handle                      Halfedge_const_handle;
  typedef typename Arrangement::Face_const_handle                          Face_const_handle;
  typedef typename Arrangement::Vertex_iterator                            Vertex_iterator;
  typedef typename Arrangement::Vertex_const_iterator                      Vertex_const_iterator;
  typedef typename Arrangement::Halfedge_iterator                          Halfedge_iterator;
  typedef typename Arrangement::Halfedge_const_iterator                    Halfedge_const_iterator;
  typedef typename Arrangement::Face_iterator                              Face_iterator;
  typedef typename Arrangement::Face_const_iterator                        Face_const_iterator;
  typedef typename Arrangement::Ccb_halfedge_circulator                    Ccb_halfedge_circulator;
  typedef typename Arrangement::Ccb_halfedge_const_circulator              Ccb_halfedge_const_circulator;
  typedef typename Arrangement::Holes_iterator                             Holes_iterator;
  typedef typename Arrangement::Holes_const_iterator                       Holes_const_iterator;
  typedef typename Arrangement::Locate_type                                Locate_type;
  typedef typename Arrangement::Traits_wrap                                Traits_wrap;
  typedef typename Arrangement::Change_notification                        Change_notification; 
  
  typedef typename Arrangement::Traits                                     Traits;
  typedef typename Traits::Point                                           Point;
  typedef typename Traits::X_curve                                         X_curve;

  typedef typename Arrangement::Planar_map                                PM;
  typedef typename PM::Vertex_handle                                      Pm_vertex_handle;
  typedef typename PM::Halfedge_handle                                    Pm_halfedge_handle;
  typedef typename PM::Face_handle                                        Pm_face_handle;
  typedef typename PM::Vertex_const_handle                                Pm_vertex_const_handle;
  typedef typename PM::Halfedge_const_handle                              Pm_halfedge_const_handle;
  typedef typename PM::Face_const_handle                                  Pm_face_const_handle;

  typedef Map_overlay_post_proc_notifier<Arrangement>        Self;
  typedef const Arrangement*                                 Arr_const_pointer;
  
  Map_overlay_post_proc_notifier() : arr1(0), arr2(0) {}

  Map_overlay_post_proc_notifier (Arr_const_pointer sub_division1, 
                                  Arr_const_pointer sub_division2) 
    : arr1(sub_division1), arr2(sub_division2) {}
  
  Map_overlay_post_proc_notifier (const Self& notf) 
    : arr1(notf.get_sub_division1()), arr2(notf.get_sub_division2()) {}
  
  Map_overlay_post_proc_notifier (const Self* notf) 
    : arr1(notf->get_sub_division1()), arr2(notf->get_sub_division2()) {}
  
  virtual ~Map_overlay_post_proc_notifier() {}

  void add_edge(const X_curve& cv, 
                Pm_halfedge_handle e, 
                bool left_to_right, 
                bool overlap = false)
  {
    //mapOverlay   tmp_ovl;
    
#ifdef CGAL_SWEEP_LINE_DEBUG
    std::cout<<"in add_edge" << std::endl;
#endif

    if ((CGAL::compare_lexicographically_xy(e->source()->point(), 
                                            e->target()->point()) == CGAL::SMALLER && 
         CGAL::compare_lexicographically_xy(orig_halfedge1->source()->point(), 
                                            orig_halfedge1->target()->point()) == CGAL::SMALLER) || 
        (CGAL::compare_lexicographically_xy(e->source()->point(), 
                                            e->target()->point()) == CGAL::LARGER && 
         CGAL::compare_lexicographically_xy(orig_halfedge1->source()->point(), 
                                            orig_halfedge1->target()->point()) == CGAL::LARGER)){
      if (first_halfedge){
        set_first_halfedge_above(Halfedge_handle(e), orig_halfedge1);
        set_first_halfedge_above(Halfedge_handle(e->twin()), orig_halfedge1->twin());
        //e->twin()->set_first_halfedge_above(orig_halfedge1->twin().operator->());
      }
      else{ 
        set_second_halfedge_above(Halfedge_handle(e), orig_halfedge1);
        set_second_halfedge_above(Halfedge_handle(e->twin()), orig_halfedge1->twin());

        //e->set_second_halfedge_above(orig_halfedge1.operator->());
        //e->twin()->set_second_halfedge_above(orig_halfedge1->twin().operator->());
      }
    }
    
    else 
      /*if ((CGAL::compare_lexicographically_xy(e->source()->point(), 
        e->target()->point()) == CGAL::SMALLER && 
        CGAL::compare_lexicographically_xy(orig_halfedge2->source()->point(), 
        orig_halfedge2->target()->point()) == CGAL::SMALLER)
        || (CGAL::compare_lexicographically_xy(e->source()->point(), 
        e->target()->point()) == CGAL::LARGER && 
        CGAL::compare_lexicographically_xy(orig_halfedge2->source()->point(), 
        orig_halfedge2->target()->point()) == CGAL::LARGER))*/
      {
        if (first_halfedge){
          set_first_halfedge_above(Halfedge_handle(e), orig_halfedge2);
          set_first_halfedge_above(Halfedge_handle(e->twin()), orig_halfedge2->twin());
          
          //e->set_first_halfedge_above(orig_halfedge2.operator->());
          //e->twin()->set_first_halfedge_above(orig_halfedge2->twin().operator->());
        }
        else{
          set_second_halfedge_above(Halfedge_handle(e), orig_halfedge2);
          set_second_halfedge_above(Halfedge_handle(e->twin()), orig_halfedge2->twin());
          
          //e->set_second_halfedge_above(orig_halfedge2.operator->());
          //e->twin()->set_second_halfedge_above(orig_halfedge2->twin().operator->());
        }
      }
    //else
    //  assert(0);
  } 
  
  void split_edge(Pm_halfedge_handle orig_edge, 
                  Pm_halfedge_handle new_edge, 
                  const X_curve& c1, 
                  const X_curve& c2)
  {
#ifdef CGAL_SWEEP_LINE_DEBUG
    std::cout<<"is split_edge" << std::endl;
    if (new_edge->get_first_halfedge_above() != NULL)
      cout<<"new blue edge ";
    if (new_edge->get_second_halfedge_above() != NULL)
      cout<<"new red edge ";
    cout<<"\n";
#endif
    
    new_edge->set_first_halfedge_above(orig_edge->get_first_halfedge_above());
    new_edge->set_second_halfedge_above(orig_edge->get_second_halfedge_above());

    new_edge->twin()->set_first_halfedge_above(orig_edge->twin()->get_first_halfedge_above());
    new_edge->twin()->set_second_halfedge_above(orig_edge->twin()->get_second_halfedge_above());

#ifdef CGAL_SWEEP_LINE_DEBUG
    if (orig_edge->get_first_halfedge_above() != NULL)
      cout<<"blue edge ";
    if (orig_edge->get_second_halfedge_above() != NULL)
      cout<<"red edge ";
    cout<<"\n";
#endif
  }


  void split_face(Face_handle orig_face, Face_handle new_face) {}
  

  void add_hole(Face_handle in_face, Halfedge_handle new_hole) {}


  const X_curve &edge_support_curve(Halfedge_handle edge)
  {
    return edge->curve();
  }
  
  bool have_support_curve()
  {
    return false;
  }

  void  update_all_faces(Arrangement& pmwx, 
                         const Arrangement& first_creator,  
                         const Arrangement& second_creator)
  {
    //mapOverlay   tmp_ovl;
    Face_handle  unbounded = pmwx.unbounded_face();

    // first taking care of the unbounded face.
    set_first_face_above(unbounded, first_creator.unbounded_face());
    set_second_face_above(unbounded, second_creator.unbounded_face());
    unbounded->set_color(Face::BLACK);

    for (Holes_iterator hit = unbounded->holes_begin(); hit != unbounded->holes_end(); ++hit) {
      Ccb_halfedge_circulator begin_halfedge(*hit);
      do {
        list<Face_handle>  faces_queue;  // a queue to hold all faces involed with hit.
        
        Face_handle begin_face = begin_halfedge->twin()->face();  // gives a face that touch the unbounded.
        if (begin_face->get_color() == Face::WHITE){
          //std::cout<<"begin edge is :" <<begin_halfedge->curve()<<"\n";

          Ccb_halfedge_circulator ccb_cir = begin_face->outer_ccb();
          
          do{
            if (ccb_cir->get_first_halfedge_above() != NULL){
#ifdef CGAL_SWEEP_LINE_DEBUG
              if (get_first_halfedge_above(ccb_cir)->face()->is_unbounded())
                std::cout<<"Face (of first halfedge) above begin_face->outer_ccb() is unbounded"<<std::endl;
              std::cout<<"ccb_cir->get_first_halfedge_above() != NULL" << std::endl;
#endif
              set_first_face_above(begin_face, get_first_halfedge_above(ccb_cir)->face());
              //begin_face->set_first_face_above (tmp_ovl.get_first_halfedge_above(ccb_cir)->face().operator->());
              //begin_face->set_first_face_above(((Halfedge*) ccb_cir->get_first_halfedge_above())->face().operator->());
            }
            if (ccb_cir->get_second_halfedge_above() != NULL){
#ifdef CGAL_SWEEP_LINE_DEBUG
              if (get_second_halfedge_above(ccb_cir)->face()->is_unbounded())
                std::cout<<"Face above (ofsecond halfedge) begin_face->outer_ccb() is unbounded"<<std::endl;
              std::cout<<"ccb_cir->get_second_halfedge_above() != NULL" << std::endl;
#endif
                
              set_second_face_above(begin_face, get_second_halfedge_above(ccb_cir)->face());
              //begin_face->set_second_face_above(tmp_ovl.get_second_halfedge_above(ccb_cir)->face().operator->());
              //begin_face->set_second_face_above( ((Halfedge*) ccb_cir->get_second_halfedge_above())->face().operator->());
            }
          } while (++ccb_cir != begin_face->outer_ccb());
          
          do{
            if (ccb_cir->get_first_face_above() != NULL && begin_face->get_first_face_above() == NULL){
#ifdef CGAL_SWEEP_LINE_DEBUG
              if (get_first_face_above(ccb_cir)->is_unbounded())
                std::cout<<"First face above ccb_cir of begin_face is unbounded"<<std::endl;
              std::cout<<"ccb_cir->get_first_face_above() != NULL" << std::endl;
#endif
                
              set_first_face_above(begin_face, get_first_face_above(ccb_cir));
              //begin_face->set_first_face_above(ccb_cir->get_first_face_above());
            }
            
            else if (ccb_cir->get_second_face_above() != NULL &&  begin_face->get_second_face_above() == NULL){
#ifdef CGAL_SWEEP_LINE_DEBUG
              if (get_second_face_above(ccb_cir)->is_unbounded())
                std::cout<<"Second face above ccb_cir of begin_face is unbounded"<<std::endl;
              cout<<"ccb_cir->get_second_face_above() != NULL" << std::endl;
#endif
              
              set_second_face_above(begin_face, get_second_face_above(ccb_cir));
              //begin_face->set_second_face_above(ccb_cir->get_second_face_above());  
            }
          } while (++ccb_cir != begin_face->outer_ccb());
          
          // checking wether one of the face pointer is NULL - if it is - it must be under an unbounded creator face.
          if (begin_face->get_first_face_above() == NULL){
#ifdef CGAL_SWEEP_LINE_DEBUG
            std::cout<<"first face above begin_face is NULL - putting unbounded"<<std::endl;
#endif
            
            set_first_face_above(begin_face, first_creator.unbounded_face());
            //begin_face->set_first_face_above(first_creator.unbounded_face().operator->());
          }
   
          if (begin_face->get_second_face_above() == NULL){
#ifdef CGAL_SWEEP_LINE_DEBUG
            std::cout<<"second face above begin_face is NULL - putting unbounded"<<std::endl;
#endif
            
            set_second_face_above(begin_face, second_creator.unbounded_face());
            //begin_face->set_second_face_above(second_creator.unbounded_face().operator->());
          }

          // inserting begin_face to queue and starting BFS.
          begin_face->set_color(Face::GRAY);
          faces_queue.push_back(begin_face);
          
#ifdef CGAL_SWEEP_LINE_DEBUG
          cout<<"Starting BFS\n";
#endif
          while ( !(faces_queue.empty()) ){
            Face_handle  face = faces_queue.front();
            
#ifdef CGAL_SWEEP_LINE_DEBUG
            cout<<"got queue face\n";
#endif
            for (Holes_iterator face_h_it = face->holes_begin(); face_h_it != face->holes_end(); ++face_h_it) {
              Ccb_halfedge_circulator face_cc(*face_h_it);
              
              do {
                Halfedge_handle next_hole_edge = face_cc->twin();  // fix it to all edges!
                Face_handle  next_hole_face = next_hole_edge->face();
                
                if (next_hole_face->get_color() == Face::WHITE){  
                  if (next_hole_edge->get_first_halfedge_above() == NULL && face->get_first_face_above() != NULL){
                    set_first_face_above(next_hole_edge, get_first_face_above(face));
                    //next_hole_edge->set_first_face_above(face->get_first_face_above());
                  }
                  
                  if (next_hole_edge->get_second_halfedge_above() == NULL && face->get_second_face_above() != NULL){
                    set_second_face_above(next_hole_edge, get_second_face_above(face));
                    //next_hole_edge->set_second_face_above(face->get_second_face_above()); 
                  }
                  
                  //------------ update the next holed face
                  
                  Ccb_halfedge_circulator next_hole_cc = next_hole_face->outer_ccb();
                  
                  do{
                    if (next_hole_cc->get_first_halfedge_above() != NULL){
                      set_first_face_above(next_hole_face, get_first_halfedge_above(next_hole_cc)->face());
                      //next_hole_face->set_first_face_above(tmp_ovl.get_first_halfedge_above(next_hole_cc)->face().operator->());
                      //next_hole_face->set_first_face_above(((Halfedge*) next_hole_cc->get_first_halfedge_above())->face().operator->());
                      //cout<<next_hole_cc->curve()<<" got blue above\n"; 
                    }
                    if (next_hole_cc->get_second_halfedge_above() != NULL){
                      set_second_face_above(next_hole_face, get_second_halfedge_above(next_hole_cc)->face());
                      //next_hole_face->set_second_face_above(tmp_ovl.get_second_halfedge_above(next_hole_cc)->face().operator->());
                      //next_hole_face->set_second_face_above(((Halfedge*) next_hole_cc->get_second_halfedge_above())->face().operator->());
                      //std::cout<<next_hole_cc->curve()<<" got red above\n";
                    }
                  } while (++next_hole_cc != next_hole_face->outer_ccb());

                  do{
                    if (next_hole_cc->get_first_face_above() != NULL && next_hole_face->get_first_face_above() == NULL){
                      set_first_face_above(next_hole_face, get_first_face_above(next_hole_cc));
                      //next_hole_face->set_first_face_above(next_hole_cc->get_first_face_above());
                      //cout<<next_hole_cc->curve()<<" got blue above\n";
                    }
                    
                    if (next_hole_cc->get_second_face_above() != NULL && next_hole_face->get_second_face_above() == NULL){
                      set_second_face_above(next_hole_face, get_second_face_above(next_hole_cc));
                      //next_hole_face->set_second_face_above(next_hole_cc->get_second_face_above());
                      //std::cout<<next_hole_cc->curve()<<" got red above\n";
                    }
                  } while (++next_hole_cc != next_hole_face->outer_ccb());

                  
                  next_hole_face->set_color(Face::GRAY);
                  faces_queue.push_back (next_hole_face);
                }
              } while (++face_cc != *face_h_it);
            }// end for loop on holes of face.
              
            
            /// taking care of the neighbors.
            Ccb_halfedge_circulator cc = face->outer_ccb();
            
            do {
              Halfedge_handle next_edge = cc->twin();
              Face_handle  next_face = next_edge->face();  // get the neighbor edge.
              
              if (next_face->get_color() == Face::WHITE){  
                if (next_edge->get_first_halfedge_above() == NULL && face->get_first_face_above() != NULL){  //there was a bug here!
                  set_first_face_above(next_edge, get_first_face_above(face));
                  //next_edge->set_first_face_above(face->get_first_face_above());
                }
                
                if (next_edge->get_second_halfedge_above() == NULL && face->get_second_face_above() != NULL){
                  set_second_face_above(next_edge, get_second_face_above(face));
                  //next_edge->set_second_face_above(face->get_second_face_above());
                }
                
                //------------ update the next face
                
                Ccb_halfedge_circulator next_cc = next_face->outer_ccb();
                
                do{
                  if (next_cc->get_first_halfedge_above() != NULL){
                    set_first_face_above(next_face, get_first_halfedge_above(next_cc)->face());
                    //next_face->set_first_face_above(tmp_ovl.get_first_halfedge_above(next_cc)->face().operator->());
                    //next_face->set_first_face_above(((Halfedge*) next_cc->get_first_halfedge_above())->face().operator->());
                    //std::cout<<next_cc->curve()<<" got blue above\n";
                  }                  
                  if (next_cc->get_second_halfedge_above() != NULL){
                    set_second_face_above(next_face, get_second_halfedge_above(next_cc)->face());
                    //next_face->set_second_face_above(tmp_ovl.get_second_halfedge_above(next_cc)->face().operator->());
                    //next_face->set_second_face_above(((Halfedge*) next_cc->get_second_halfedge_above())->face().operator->());
                    //std::cout<<next_cc->curve()<<" got red above\n";
                  }
                } while (++next_cc != next_face->outer_ccb());
                
                do{ // now updating next_face with faces above it that came from its neighbors.
                  if (next_cc->get_first_face_above() != NULL && next_face->get_first_face_above() == NULL){
                    //Face* f = (Face *) (next_cc->get_first_face_above());
                    //if (!(f->is_unbounded()))  // this line crashes.
                    //  cout<<"face to next_cc side is bounded\n";

                   

                    set_first_face_above(next_face, get_first_face_above(next_cc));
                    //next_face->set_first_face_above(next_cc->get_first_face_above());  
                  }
                  if (next_cc->get_second_face_above() != NULL && next_face->get_second_face_above() == NULL){
                    //cout<<"bogi red face1\n";
                    
                    set_second_face_above(next_face, get_second_face_above(next_cc));
                    
                    //next_face->set_second_face_above(next_cc->get_second_face_above());  
                    //cout<<"bogi red face2\n";
                  }
                } while (++next_cc != next_face->outer_ccb());
                
                next_face->set_color(Face::GRAY);
                faces_queue.push_back (next_face);
              }
            } while (++cc != face->outer_ccb()); // end loop on neighbors of face.
            
            face->set_color(Face::BLACK);
            faces_queue.pop_front();
            
#ifdef CGAL_SWEEP_LINE_DEBUG
            cout<<"face was poped\n";
#endif
          }
        } //end if (the begin_face was white).
      } while (++begin_halfedge != *hit);
    }
  }

  /***************************************** new functions **********************************************************/
  void set_curve_attributes(const X_curve& cv, 
                            Halfedge_const_handle orig_halfedge1_, 
                            bool first_halfedge_)
  {
    orig_halfedge1 = orig_halfedge1_;
    orig_halfedge2 = orig_halfedge1_->twin();
    first_halfedge = first_halfedge_;
  }
  
  Arr_const_pointer first_subdivision () const { return arr1;}
  
  Arr_const_pointer second_subdivision () const { return arr2;}
  
  //-----------------------------------------  handle wrappering.
  // setting the vertex above.
  void  set_first_vertex_above(Vertex_handle v, Vertex_handle v_above) {
    v->set_first_vertex_above(v_above.operator->());
  }

  void  set_second_vertex_above(Vertex_handle v, Vertex_handle v_above) {
    v->set_second_vertex_above(v_above.operator->());
  }

  void  set_first_vertex_above(Vertex_handle v, Vertex_const_handle v_above) {
    v->set_first_vertex_above(v_above.operator->());
  }

  void  set_second_vertex_above(Vertex_handle v, Vertex_const_handle v_above) {
    v->set_second_vertex_above(v_above.operator->());
  }

  // setting the halfedge above.
  void  set_first_halfedge_above(Vertex_handle v, Halfedge_handle h_above) {
    v->set_first_halfedge_above(h_above.operator->());
  }

  void  set_second_halfedge_above(Vertex_handle v, Halfedge_handle h_above) {
    v->set_second_halfedge_above(h_above.operator->());
  }

  void  set_first_halfedge_above(Vertex_handle v, Halfedge_const_handle h_above) {
    v->set_first_halfedge_above(h_above.operator->());
  }
  
  void  set_second_halfedge_above(Vertex_handle v, Halfedge_const_handle h_above) {
    v->set_second_halfedge_above(h_above.operator->());
  }

  void  set_first_halfedge_above(Halfedge_handle h, Halfedge_handle h_above) {
    h->set_first_halfedge_above(h_above.operator->());
  }

  void  set_second_halfedge_above(Halfedge_handle h, Halfedge_handle h_above) {
    h->set_second_halfedge_above(h_above.operator->());
  }

  void  set_first_halfedge_above(Halfedge_handle h, Halfedge_const_handle h_above) {
    h->set_first_halfedge_above(h_above.operator->());
  }
  
  void  set_second_halfedge_above(Halfedge_handle h, Halfedge_const_handle h_above) {
    h->set_second_halfedge_above(h_above.operator->());
  }
  
  // setting the faces above.
  void  set_first_face_above(Vertex_handle v, Face_handle f_above) {
    v->set_first_face_above(f_above.operator->());
  }

  void  set_second_face_above(Vertex_handle v, Face_handle f_above) {
    v->set_second_face_above(f_above.operator->());
  }
  
  void  set_first_face_above(Vertex_handle v, Face_const_handle f_above) {
    v->set_first_face_above(f_above.operator->());
  }
  
  void  set_second_face_above(Vertex_handle v, Face_const_handle f_above) {
    v->set_second_face_above(f_above.operator->());
  }

  void  set_first_face_above(Halfedge_handle h, Face_handle f_above) {
    h->set_first_face_above(f_above.operator->());
  }

  void  set_second_face_above(Halfedge_handle h, Face_handle f_above) {
    h->set_second_face_above(f_above.operator->());
  }

  void  set_first_face_above(Halfedge_handle h, Face_const_handle f_above) {
    h->set_first_face_above(f_above.operator->());
  }
  
  void  set_second_face_above(Halfedge_handle h, Face_const_handle f_above) {
    h->set_second_face_above(f_above.operator->());
  }
  
  void  set_first_face_above(Face_handle f, Face_handle f_above) {
    f->set_first_face_above(f_above.operator->());
  }

  void  set_second_face_above(Face_handle f, Face_handle f_above) {
    f->set_second_face_above(f_above.operator->());
  }

  void  set_first_face_above(Face_handle f, Face_const_handle f_above) {
    f->set_first_face_above(f_above.operator->());
  }
  
  void  set_second_face_above(Face_handle f, Face_const_handle f_above) {
    f->set_second_face_above(f_above.operator->());
  }

  // getting the vertex above.
  Vertex_handle get_first_vertex_above(Vertex_handle v) {
    Vertex* vp = (Vertex*) v->get_first_vertex_above() ;
    
    if (vp){
      Pm_vertex_handle tmp_v = Pm_vertex_handle(vp);
      return Vertex_handle(tmp_v);
    }
    else
      return v;
  }
    
  Vertex_handle get_second_vertex_above(Vertex_handle v) {
    Vertex* vp = (Vertex*) v->get_second_vertex_above() ;
    
    if (vp){
      Pm_vertex_handle tmp_v = Pm_vertex_handle(vp);
      return Vertex_handle(tmp_v);
    }
    else
      return v;
  }
  
  Vertex_const_handle get_first_vertex_above(Vertex_const_handle v) {
    Vertex* vp = (Vertex*) v->get_first_vertex_above() ;
    
    if (vp){
      Pm_vertex_const_handle tmp_v = Pm_vertex_const_handle(vp);
      return Vertex_const_handle(tmp_v);
    }
    else
      return v;
  }
  
  Vertex_const_handle get_second_vertex_above(Vertex_const_handle v) {
    Vertex* vp = (Vertex*) v->get_second_vertex_above() ;
    
    if (vp){
      Pm_vertex_const_handle tmp_v = Pm_vertex_const_handle(vp);
      return Vertex_const_handle(tmp_v);
    }
    else
      return v;
  }
  
  // getting the halfedge above.
  Halfedge_handle get_first_halfedge_above(Vertex_handle v) {
    Halfedge* hp = (Halfedge*) v->get_first_halfedge_above() ;
    
    if (hp){
      Pm_halfedge_handle tmp_h = Pm_halfedge_handle(hp);
      return Halfedge_handle(tmp_h);
    }
    else
      return v->incident_halfedges();
  }
   
  Halfedge_handle get_second_halfedge_above(Vertex_handle v) {
    Halfedge* hp = (Halfedge*) v->get_second_halfedge_above() ;

    if (hp){
      Pm_halfedge_handle tmp_h = Pm_halfedge_handle(hp);
      return Halfedge_handle(tmp_h);
    }
    else
      return v->incident_halfedges();
  }
  
  Halfedge_const_handle get_first_halfedge_above(Vertex_const_handle v) {
    Halfedge* hp = (Halfedge*) v->get_first_halfedge_above() ;
    
    if (hp){
      Pm_halfedge_const_handle tmp_h = Pm_halfedge_const_handle(hp);
      return Halfedge_const_handle(tmp_h);
    }
    else
      return v->incident_halfedges();
  }
   
  Halfedge_const_handle get_second_halfedge_above(Vertex_const_handle v) {
    Halfedge* hp = (Halfedge*) v->get_second_halfedge_above() ;
    
    if (hp){
      Pm_halfedge_const_handle tmp_h = Pm_halfedge_const_handle(hp);
      return Halfedge_const_handle(tmp_h);
    }
    else
      return v->incident_halfedges();
  }

  Halfedge_handle get_first_halfedge_above(Halfedge_handle h) {
    Halfedge* hp = (Halfedge*) h->get_first_halfedge_above() ;
    
    if (hp){
      Pm_halfedge_handle tmp_h = Pm_halfedge_handle(hp);
      return Halfedge_handle(tmp_h);
    }
    else
      return h;
  }
   
  Halfedge_handle get_second_halfedge_above(Halfedge_handle h) {
    Halfedge* hp = (Halfedge*) h->get_second_halfedge_above() ;

    if (hp){
      Pm_halfedge_handle tmp_h = Pm_halfedge_handle(hp);
      return Halfedge_handle(tmp_h);
    }
    else
      return h;
  }

  Halfedge_const_handle get_first_halfedge_above(Halfedge_const_handle h) {
    Halfedge* hp = (Halfedge*) h->get_first_halfedge_above() ;
    
    if (hp){
      Pm_halfedge_const_handle tmp_h = Pm_halfedge_const_handle(hp);
      return Halfedge_const_handle(tmp_h);
    }
    else
      return h;
  }
   
  Halfedge_const_handle get_second_halfedge_above(Halfedge_const_handle h) {
    Halfedge* hp = (Halfedge*) h->get_second_halfedge_above() ;
    
    if (hp){
      Pm_halfedge_const_handle tmp_h = Pm_halfedge_const_handle(hp);
      return Halfedge_const_handle(tmp_h);
    }
    else
      return h;
  }

  // getting the face above.

  Face_handle get_first_face_above(Vertex_handle v) {
    Face* fp = (Face*) v->get_first_face_above() ;
    
    if (fp){
      Pm_face_handle tmp_f = Pm_face_handle(fp);
      return Face_handle(tmp_f);
    }
    else
      return v->incident_halfedges()->face();
  }
  
  Face_handle get_second_face_above(Vertex_handle v) {
    Face* fp = (Face*) v->get_second_face_above() ;
    
    if (fp){
      Pm_face_handle tmp_f = Pm_face_handle(fp);
      return Face_handle(tmp_f);
    }
    else
      return v->incident_halfedges()->face();
  }

  Face_const_handle get_first_face_above(Vertex_const_handle v) {
    Face* fp=(Face*) v->get_first_face_above() ;
    
    if (fp){
      Pm_face_const_handle tmp_f = Pm_face_const_handle(fp);
      return Face_const_handle(tmp_f);
    }
    else
      return v->incident_halfedges()->face();
  }
  
  Face_const_handle get_second_face_above(Vertex_const_handle v) {
    Face* fp=(Face*) v->get_second_face_above() ;
    
    if (fp){
      Pm_face_const_handle tmp_f = Pm_face_const_handle(fp);
      return Face_const_handle(tmp_f);
    }
    else
      return v->incident_halfedges()->face();
  }

  Face_handle get_first_face_above(Halfedge_handle h) {
    Face* fp = (Face*) h->get_first_face_above() ;
    
    if (fp){
      Pm_face_handle tmp_f = Pm_face_handle(fp);
      return Face_handle(tmp_f);
    }
    else
      return h->face();
  }
  
  Face_handle get_second_face_above(Halfedge_handle h) {
    Face* fp = (Face*) h->get_second_face_above() ;
    
    if (fp){
      Pm_face_handle tmp_f = Pm_face_handle(fp);
      return Face_handle(tmp_f);
    }
    else
      return h->face();
  }

  Face_const_handle get_first_face_above(Halfedge_const_handle h) {
    Face* fp=(Face*) h->get_first_face_above() ;
    
    if (fp){
      Pm_face_const_handle tmp_f = Pm_face_const_handle(fp);
      return Face_const_handle(tmp_f);
    }
    else
      return h->face();
  }
  
  Face_const_handle get_second_face_above(Halfedge_const_handle h) {
    Face* fp=(Face*) h->get_second_face_above() ;
    
    if (fp){
      Pm_face_const_handle tmp_f = Pm_face_const_handle(fp);
      return Face_const_handle(tmp_f);
    }
    else
      return h->face();
  }

  Face_handle get_first_face_above(Face_handle f) {
    Face* fp = (Face*) f->get_first_face_above() ;
    
    if (fp){
      Pm_face_handle tmp_f = Pm_face_handle(fp);
      return Face_handle(tmp_f);
    }
    else
      return f;
  }
  
  Face_handle get_second_face_above(Face_handle f) {
    Face* fp = (Face*) f->get_second_face_above() ;
    
    if (fp){
      Pm_face_handle tmp_f = Pm_face_handle(fp);
      return Face_handle(tmp_f);
    }
    else
      return f;
  }
  
  Face_const_handle get_first_face_above(Face_const_handle f) {
    Face* fp = (Face*) f->get_first_face_above() ;
    
    if (fp){
      Pm_face_const_handle tmp_f = Pm_face_const_handle(fp);
      return Face_const_handle(tmp_f);
    }
    else
      return f;
  }
  
  Face_const_handle get_second_face_above(Face_const_handle f) {
    Face* fp=(Face*) f->get_second_face_above() ;
 
    if (fp){
      Pm_face_const_handle tmp_f = Pm_face_const_handle(fp);
      return Face_const_handle(tmp_f);
    }
    else
      return f;
  }
   
  //---------------------------- end handles wrappering.
  
private:
  bool                   first_halfedge;
  Halfedge_const_handle  orig_halfedge1, orig_halfedge2;
  Arr_const_pointer      arr1, arr2;
};

CGAL_END_NAMESPACE

#endif
