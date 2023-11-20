const mongoose = require('mongoose')

const ComplaintSchema = mongoose.Schema({
    type: {
        type: String
    },
    registeredby: {
        type: String
    },
    hostel:{
        type: String
    },
    room:{
        type:Number
    },
    contact: {
        type: String
    },
    status:{
        type: String
    },
    slot:{
        type: String
    },
    date:{
        type: Date,
        default: Date.now
    },
    // registeredby:{
    //     type:String
        
    // }
    desc: {
        type: String
    }
});

const Complaint = mongoose.model('Complaint', ComplaintSchema);
module.exports = Complaint;

// const mongoose = require('mongoose')

// const ComplaintSchema = mongoose.Schema({
//     name: {
//         type: String
//     },
//     email: {
//         type: String
//     },
//     contact: {
//         type: String
//     },
//     desc: {
//         type: String
//     }
// });

// const Complaint = module.exports = mongoose.model('Complaint', ComplaintSchema);

// module.exports.registerComplaint = function (newComplaint, callback) {
//     newComplaint.save(callback);
// }

// module.exports.getAllComplaints = function(callback){
//     Complaint.find(callback);
//   }