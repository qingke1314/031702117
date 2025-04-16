import {
  BrowserRouter as Router,
  Route,
  Routes,
  Navigate,
} from 'react-router-dom';
import { Navbar } from './components/Navbar';
import { PostsList } from '@/pages/PostsList/index';
import { SinglePostPage } from '@/pages/SinglePost/index';
import { EditPostForm } from '@/pages/EditPostForm/index';
import { AddPostForm } from '@/pages/AddPostForm/index';

function App() {
  return (
    <Router>
      <Navbar />
      <div className="App">
        <Routes>
          <Route
            path="/"
            element={
              <>
                <AddPostForm />
                <PostsList />
              </>
            }
          ></Route>
          <Route path="/posts/:postId" element={<SinglePostPage />} />
          <Route path="/editPost/:postId" element={<EditPostForm />} />
          <Route path="*" element={<Navigate to="/"></Navigate>}></Route>
        </Routes>
      </div>
    </Router>
  );
}

export default App;
