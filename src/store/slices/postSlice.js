import { createSlice } from '@reduxjs/toolkit';

const initialState = [
  {
    id: '1',
    title: 'first post',
    content: 'Hello!',
  },
  {
    id: '2',
    title: 'second post',
    content: 'More text',
  },
];

const postsSlice = createSlice({
  name: 'posts',
  initialState,
  reducers: {
    postsAdd: {
      reducer(state, action) {
        state.push(action.payload);
      },
      prepare({ title, content, userId }) {
        return {
          payload: {
            id: nanoid(),
            title,
            content,
            user: userId,
          },
        };
      },
    },
    postUpdated(state, action) {
      const { id, title, content } = action.payload;
      const existingPost = state.find((post) => post.id === id);
      if (existingPost) {
        existingPost.title = title;
        existingPost.content = content;
      }
    },
  },
});

export const { postsAdd, postUpdated } = postsSlice.actions;

export default postsSlice.reducer;
