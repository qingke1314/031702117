import React, { useState } from 'react';
import { postsAdd } from '@/store/slices/postSlice';
import { useDispatch, useSelector } from 'react-redux';
import { nanoid } from '@reduxjs/toolkit';
export const AddPostForm = () => {
  const [userId, setUserId] = useState('');
  const dispatch = useDispatch();
  const [title, setTitle] = useState('');
  const [content, setContent] = useState('');
  const users = useSelector((state) => state.users);

  const onTitleChanged = (e) => setTitle(e.target.value);
  const onContentChanged = (e) => setContent(e.target.value);
  const onSavePostClicked = () => {
    if (title && content) {
      dispatch(
        postsAdd({
          title,
          content,
          userId,
        }),
      );
      setTitle('');
      setContent('');
    }
  };
  const canSave = Boolean(title) && Boolean(content) && Boolean(userId);

  const usersOptions = users.map((user) => (
    <option key={user.id} value={user.id}>
      {user.name}
    </option>
  ));
  const onAuthorChanged = (e) => setUserId(e.target.value);
  return (
    <section>
      <h2>添加新文章</h2>
      <form>
        <label htmlFor="postTitle">文章标题:</label>
        <input
          type="text"
          id="postTitle"
          name="postTitle"
          value={title}
          onChange={onTitleChanged}
        />
        <label htmlFor="postAuthor">Author:</label>
        <select id="postAuthor" value={userId} onChange={onAuthorChanged}>
          <option value=""></option>
          {usersOptions}
        </select>
        <label htmlFor="postContent">内容：</label>
        <textarea
          id="postContent"
          name="postContent"
          value={content}
          onChange={onContentChanged}
        />
        <button type="button" onClick={onSavePostClicked} disabled={!canSave}>
          保存文章
        </button>
      </form>
    </section>
  );
};
