import { configureStore } from '@reduxjs/toolkit';

import postsReducer from '@/store/slices/postSlice';
import usersReducer from '@/store/slices/userSlice';

export default configureStore({
  reducer: {
    posts: postsReducer,
    users: usersReducer,
  },
});
