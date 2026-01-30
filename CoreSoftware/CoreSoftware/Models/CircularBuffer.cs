public class CircularBuffer<T>
{
    private readonly T[] _buffer;
    private int _head; // Index to read from
    private int _tail; // Index to write to
    private int _count;
    private readonly int _capacity;

    public int Capacity => _capacity;
    public int Count => _count;

    public CircularBuffer(int capacity)
    {
        _capacity = capacity;
        _buffer = new T[capacity];
        _head = 0;
        _tail = 0;
        _count = 0;
    }

    public void Write(T value)
    {
        _buffer[_tail] = value;
        _tail = (_tail + 1) % _capacity;

        if (_count == _capacity)
        {
            _head = _tail; 
        }
        else
        {
            _count++;
        }
    }

    public T Read()
    {
        if (_count == 0)
        {
            throw new InvalidOperationException("Buffer is empty.");
        }

        T value = _buffer[_head];
        _buffer[_head] = default(T); 
        _head = (_head + 1) % _capacity;
        _count--;

        return value;
    }

    public void Clear()
    {
        Array.Clear(_buffer, 0, _buffer.Length);
        _head = 0;
        _tail = 0;
        _count = 0;
    }

    public T[] ToArray()
    {
        if (_count == 0) return Array.Empty<T>();
        T[] result = new T[_count];
        for (int head = _head, i = 0; i < _count ; i++,head++)
        {
            result[i] = _buffer[head % _capacity];
        }
        return result;
    }
}